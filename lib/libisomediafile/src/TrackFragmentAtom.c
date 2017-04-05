/*
This software module was originally developed by Apple Computer, Inc.
in the course of development of MPEG-4. 
This software module is an implementation of a part of one or 
more MPEG-4 tools as specified by MPEG-4. 
ISO/IEC gives users of MPEG-4 free license to this
software module or modifications thereof for use in hardware 
or software products claiming conformance to MPEG-4.
Those intending to use this software module in hardware or software
products are advised that its use may infringe existing patents.
The original developer of this software module and his/her company,
the subsequent editors and their companies, and ISO/IEC have no
liability for use of this software module or modifications thereof
in an implementation.
Copyright is not released for non MPEG-4 conforming
products. Apple Computer, Inc. retains full right to use the code for its own
purpose, assign or donate the code to a third party and to
inhibit third parties from using the code for non 
MPEG-4 conforming products.
This copyright notice must be included in all copies or
derivative works. Copyright (c) 1999.
*/
/*
	$Id: TrackFragmentAtom.c,v 1.1.1.1 2002/09/20 08:53:35 julien Exp $
*/

#include "MP4Atoms.h"
#include <stdlib.h>


static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4TrackFragmentAtomPtr self;

	self = (MP4TrackFragmentAtomPtr) s;
	if ( self == NULL ) BAILWITHERROR( MP4BadParamErr )
	DESTROY_ATOM_LIST_F( atomList );
	DESTROY_ATOM_LIST_F( groupList );
	(self->tfhd)->destroy( (MP4AtomPtr) (self->tfhd) );
	
	if ( self->super )
		self->super->destroy( s );

bail:
	TEST_RETURN( err );

	return;
}

static MP4Err addAtom( MP4TrackFragmentAtomPtr self, MP4AtomPtr atom )
{
	MP4Err err;
	err = MP4NoErr;
	
	if ( self == 0 )
		BAILWITHERROR( MP4BadParamErr );
	switch (atom->type) {
		case MP4TrackFragmentHeaderAtomType: self->tfhd = atom; break;
		case MP4TrackRunAtomType: err = MP4AddListEntry( atom, self->atomList ); break;
		case MP4SampletoGroupAtomType: err = MP4AddListEntry( atom, self->groupList ); break;
		default: BAILWITHERROR( MP4BadDataErr )
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err mdatMoved( struct MP4MediaInformationAtom* s, u64 mdatBase, u64 mdatEnd, s32 mdatOffset )
{
	MP4Err err;
	MP4TrackFragmentAtomPtr self;
	MP4TrackFragmentHeaderAtomPtr tfhd;
	u64 mdatSize;
	
	self = (MP4TrackFragmentAtomPtr) s;
	
	err = MP4NoErr;
	mdatSize = mdatEnd - mdatBase;	/* not something we need to know, but it avoids an unused param warning */
	
	tfhd = (MP4TrackFragmentHeaderAtomPtr) self->tfhd;  
	if (tfhd==NULL) BAILWITHERROR( MP4BadParamErr )

	if (self->samples_use_mdat == 1) tfhd->base_data_offset = mdatOffset;
	
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err addSampleGroups( struct MP4TrackFragmentAtom *self, u32 sampleCount )
{
	MP4Err err;
	u32 groupListSize, i;
	
	err = MP4GetListEntryCount( self->groupList, &groupListSize ); if (err) goto bail;
	for ( i = 0; i < groupListSize; i++ )
	{
		MP4SampletoGroupAtomPtr theGroup;
		err = MP4GetListEntry( self->groupList, i, (char **) &theGroup ); if (err) goto bail;
		if ( theGroup ) {
			err = theGroup->addSamples( theGroup, sampleCount );
			if (err) goto bail;
		}
	}
	
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err addSamples( struct MP4MediaInformationAtom *s, MP4Handle sampleH, 
					u32 sampleCount, MP4Handle durationsH, MP4Handle sizesH,
					MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH, MP4Handle padsH )
{
	MP4Err MP4CreateTrackRunAtom( MP4TrackRunAtomPtr *outAtom );
        MP4Err err;
	MP4MediaDataAtomPtr mdat;
	MP4TrackRunAtomPtr trun;
	MP4TrackRunEntryPtr entry;
	u32 duration_count;
	u32* durations;
	u32 size_count;
	u32* sizes;
	u32 sync_count;
	u32* syncs;
	u32 i;
	u32* decodes;
	MP4TrackFragmentAtomPtr self;
		
	self = (MP4TrackFragmentAtomPtr) s;
	
	if (sampleEntryH != NULL) BAILWITHERROR( MP4BadDataErr )
	
	if ((self->samples_use_mdat == 0) || (self->samples_use_mdat == 1)) self->samples_use_mdat = 1;
	else BAILWITHERROR( MP4BadDataErr )
	
	mdat = self->mdat;
	
	err = MP4CreateTrackRunAtom( &trun ); if (err) goto bail;
	trun->data_offset = (u32) mdat->dataSize;
	trun->samplecount = sampleCount;
	
	mdat->addData( mdat, sampleH );
	
	trun->entries = (MP4TrackRunEntryPtr) calloc( sampleCount, sizeof(MP4TrackRunEntry) );
	TESTMALLOC( trun->entries )
	
	MP4GetHandleSize( durationsH, &duration_count ); duration_count /= 4;
	
	MP4GetHandleSize( sizesH, &size_count); size_count /= 4;
	
	durations = (u32 *) *durationsH;
	sizes     = (u32 *) *sizesH;
	decodes = NULL;
	if (decodingOffsetsH != NULL) decodes   = (u32 *) *decodingOffsetsH;
	
	for (i=0, entry=trun->entries; i<sampleCount; i++, entry++) 
	{
		entry->sample_duration = ( (i>=duration_count) ? durations[0] : durations[i] );
		entry->sample_size     = ( (i>=size_count)     ?     sizes[0] :     sizes[i] );
		entry->sample_flags =
			( (padsH == NULL) ? 0 : (((u8*) padsH)[i])<<17 ) +
			( (syncSamplesH == NULL) ? 0 : fragment_difference_sample_flag );
		entry->sample_composition_time_offset =
			( (decodingOffsetsH == NULL) ? 0 : decodes[i] );
	}
	
	entry=trun->entries;
	
	if (syncSamplesH != NULL) {
		MP4GetHandleSize( syncSamplesH, &sync_count); sync_count /= 4;
		syncs = (u32*) *syncSamplesH;
		for (i=0; i<sync_count; i++)
		{
			entry[ syncs[i]-1 ].sample_flags ^= fragment_difference_sample_flag;
		}
	}
	
	err = MP4AddListEntry( trun, self->atomList ); if (err) goto bail;
	
	err = addSampleGroups( self, sampleCount ); if (err) goto bail;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err addSampleReference( struct MP4MediaInformationAtom *s, u64 dataOffset, u32 sampleCount,
				    MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH, MP4Handle padsH )
{
	MP4Err MP4CreateTrackRunAtom( MP4TrackRunAtomPtr *outAtom );
        MP4Err err;

	MP4TrackRunAtomPtr trun;
	MP4TrackRunEntryPtr entry;
	u32 duration_count;
	u32* durations;
	u32 size_count;
	u32* sizes;
	u32 sync_count;
	u32* syncs;
	u32 i;
	MP4TrackFragmentAtomPtr self;
	MP4TrackFragmentHeaderAtomPtr tfhd;
		
	self = (MP4TrackFragmentAtomPtr) s;
	
	if (sampleEntryH != NULL) BAILWITHERROR( MP4BadDataErr )
	
	if ((self->samples_use_mdat == 0) || (self->samples_use_mdat == 2)) self->samples_use_mdat = 2;
	else BAILWITHERROR( MP4BadDataErr )
	
	err = MP4CreateTrackRunAtom( &trun ); if (err) goto bail;
	
	tfhd = (MP4TrackFragmentHeaderAtomPtr) self->tfhd;
	if (tfhd->base_data_offset == 0) tfhd->base_data_offset = dataOffset;
	
	trun->data_offset = (u32) (dataOffset - (tfhd->base_data_offset));
	trun->samplecount = sampleCount;
	
	trun->entries = (MP4TrackRunEntryPtr) calloc( sampleCount, sizeof(MP4TrackRunEntry) );
	TESTMALLOC( trun->entries )
	
	MP4GetHandleSize( durationsH, &duration_count ); duration_count /= 4;	
	MP4GetHandleSize( sizesH, &size_count); size_count /= 4;
	
	durations = (u32 *) *durationsH;
	sizes     = (u32 *) *sizesH;
	
	for (i=0, entry=trun->entries; i<sampleCount; i++, entry++) 
	{
		entry->sample_duration = ( (i>=duration_count) ? durations[0] : durations[i] );
		entry->sample_size     = ( (i>=size_count)     ?     sizes[0] :     sizes[i] );
		entry->sample_flags =
			( (padsH == NULL) ? 0 : (((u8*) padsH)[i])<<13 ) +
			( (syncSamplesH == NULL) ? 0 : fragment_difference_sample_flag );
		entry->sample_composition_time_offset =
			( (decodingOffsetsH == NULL) ? 0 : ((u32*) decodingOffsetsH)[i] );
	}
	
	entry=trun->entries;
	
	if (syncSamplesH != NULL) {
		MP4GetHandleSize( syncSamplesH, &sync_count); sync_count /= 4;
		syncs = (u32*) *syncSamplesH;
		for (i=0; i<sync_count; i++)
		{
			entry[ syncs[i-1] ].sample_flags ^= fragment_difference_sample_flag;
		}
	}
	
	err = MP4AddListEntry( trun, self->atomList ); if (err) goto bail;

	err = addSampleGroups( self, sampleCount ); if (err) goto bail;

bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
	MP4Err err;
	MP4TrackFragmentAtomPtr self = (MP4TrackFragmentAtomPtr) s;
	err = MP4NoErr;
	
	if ( self->size > 0 ) {
		err = MP4SerializeCommonBaseAtomFields( s, buffer ); if (err) goto bail;
    	buffer += self->bytesWritten;	
    	SERIALIZE_ATOM( tfhd );
    	SERIALIZE_ATOM_LIST( atomList );
		SERIALIZE_ATOM_LIST( groupList );
		assert( self->bytesWritten == self->size );
	}

bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err calculateSize( struct MP4Atom* s )
{
	MP4Err err;
	MP4TrackFragmentAtomPtr self = (MP4TrackFragmentAtomPtr) s;
	MP4TrackFragmentHeaderAtomPtr tfhd;
	u32 tfhd_flags;
	u32 i;
	u32 atomListSize;

	err = MP4NoErr;
	
	tfhd = (MP4TrackFragmentHeaderAtomPtr) self->tfhd;  

	tfhd->default_sample_duration = 0;
	tfhd->default_sample_flags    = 0;
	tfhd->default_sample_size     = 0;
	tfhd_flags = tfhd_base_data_offset_present;
	
	if ( self->atomList ) {
		err = MP4GetListEntryCount( self->atomList, &atomListSize ); if (err) goto bail;
	}
	else atomListSize = 0;
	
	if (atomListSize > 0)
	{
		MP4TrackRunAtomPtr a;
		
		/* first, calculate what defaults are suitable in this track fragment header */
		
		for ( i = 0; i < atomListSize; i++ )
		{
			err = MP4GetListEntry( self->atomList, i, (char **) &a ); if (err) goto bail;
			if ( a )
				a->calculateDefaults( a, tfhd, 2 );
				/* first iteration gets the second flag value as the first can be special-cased */
		}
	
		/* then come back round and pick up the flags if not already set from second position */
		if (tfhd->default_sample_flags == 0)
		{
			for ( i = 0; i < atomListSize; i++ )
			{
				err = MP4GetListEntry( self->atomList, i, (char **) &a ); if (err) goto bail;
				if ( a )
					a->calculateDefaults( a, tfhd, 1 );
					/* pick up flags from the first position */
			}
		}
		
		/* then see how they compare with the trex defaults */
		
		if (self->default_sample_description_index != tfhd->sample_description_index)
			tfhd_flags |= tfhd_sample_description_index_present;
		if (self->default_sample_duration != tfhd->default_sample_duration)
			tfhd_flags |= tfhd_default_sample_duration_present;
		if (self->default_sample_size != tfhd->default_sample_size)
			tfhd_flags |= tfhd_default_sample_size_present;
		if (self->default_sample_flags != tfhd->default_sample_flags)
			tfhd_flags |= tfhd_default_sample_flags_present;

		/* finally, tell each run to set its flags based on what we calculated */
		for ( i = 0; i < atomListSize; i++ )
		{
			err = MP4GetListEntry( self->atomList, i, (char **) &a ); if (err) goto bail;
			if ( a )
				a->setFlags( a, tfhd );
		}
		tfhd->flags = tfhd_flags;

		err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
		ADD_ATOM_SIZE( tfhd );
		ADD_ATOM_LIST_SIZE( atomList );
		ADD_ATOM_LIST_SIZE( groupList );
	}
	else self->size = 0;
	
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err mergeRuns( MP4TrackFragmentAtomPtr self, MP4MediaAtomPtr mdia )
{
	MP4Err err;
	MP4TrackFragmentHeaderAtomPtr tfhd;
	MP4MediaInformationAtomPtr minf;
	MP4SampleTableAtomPtr stbl;
	u32 i, total_samples;

	err = MP4NoErr;
	total_samples = 0;

	tfhd = (MP4TrackFragmentHeaderAtomPtr) self->tfhd;
	minf = (MP4MediaInformationAtomPtr) mdia->information;
	stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
	
	if ( self->atomList )
	{
		u32 atomListSize;
		err = MP4GetListEntryCount( self->atomList, &atomListSize ); if (err) goto bail;
		for ( i = 0; i < atomListSize; i++ )
		{
			MP4TrackRunAtomPtr trun;
			err = MP4GetListEntry( self->atomList, i, (char **) &trun ); if (err) goto bail;
			if ( trun ) {
				u32 count;
				MP4Handle durationsH;
				MP4Handle sizesH;
				MP4Handle decodingOffsetsH;
				MP4Handle syncSamplesH;
				MP4Handle padsH;
				u32* durations;
				u32* sizes;
				u32* decodingOffsets;
				u32* syncSamples;
				u8*  pads;
				u32 havepads;
				u32 synccount;
				u32 all_sync;
				MP4TrackRunEntryPtr entry;
				u32 flags;
				u32 j;
				
				havepads = 0; synccount = 0; all_sync = 1;
				
				count=(trun->samplecount)*4;
				err = MP4NewHandle( count, &durationsH); if (err) goto bail;
				err = MP4NewHandle( count, &sizesH); if (err) goto bail;
				err = MP4NewHandle( count, &decodingOffsetsH); if (err) goto bail;
				err = MP4NewHandle( count, &syncSamplesH); if (err) goto bail;
				count /= 4;
				total_samples += count;
				
				err = MP4NewHandle( count, &padsH); if (err) goto bail;
				
				flags = trun->flags; entry = trun->entries;
				if ((flags & trun_first_sample_flags_present) != 0)
					entry->sample_flags = trun->first_sample_flags;
				
				durations = (u32*) *durationsH; sizes = (u32*) *sizesH;
				decodingOffsets = (u32*) *decodingOffsetsH;
				syncSamples = (u32*) *syncSamplesH; pads = (u8*) *padsH;
				
				for (j=0; j<count; j++, entry++, durations++, sizes++, decodingOffsets++, pads++) {
					*durations = ((flags & trun_sample_duration_present) != 0 ? entry->sample_duration : tfhd->default_sample_duration);
					*sizes = ((flags & trun_sample_size_present) != 0 ? entry->sample_size : tfhd->default_sample_size);
					*decodingOffsets = entry->sample_composition_time_offset;
					if ((j==0) && ((flags & trun_first_sample_flags_present) != 0))
						entry->sample_flags = trun->first_sample_flags;
						else if ((flags & trun_sample_flags_present) == 0) 
							entry->sample_flags = tfhd->default_sample_flags;
							
					*pads = (entry->sample_flags >> 17) & 7; /* used to be > 13, and should be a right-shift 17 anded with 7 */
					if (*pads != 0) havepads = 1;
					if ((entry->sample_flags & fragment_difference_sample_flag) == 0)
						syncSamples[ synccount++ ] = j+1;
						else all_sync = 0;
				}
				MP4SetHandleSize( syncSamplesH, synccount );
				
				stbl->setDefaultSampleEntry( stbl, tfhd->sample_description_index );
				
				err = mdia->addSampleReference( mdia, 
						tfhd->base_data_offset + trun->data_offset, 
						count, durationsH,  sizesH, NULL,
				    	((flags & trun_sample_composition_times_present) != 0 ? decodingOffsetsH : NULL), 
				    	(all_sync == 0 ? syncSamplesH : NULL), 
				    	(havepads!=0 ? padsH : NULL) );
				if (err) goto bail;
				
				err = MP4DisposeHandle( durationsH); if (err) goto bail;
				err = MP4DisposeHandle( sizesH); if (err) goto bail;
				err = MP4DisposeHandle( decodingOffsetsH); if (err) goto bail;
				err = MP4DisposeHandle( syncSamplesH); if (err) goto bail;
				err = MP4DisposeHandle( padsH); if (err) goto bail;
			}
		}
	}
	
	if (self->groupList) {
		u32 groupListSize;
		err = MP4GetListEntryCount( self->groupList, &groupListSize ); if (err) goto bail;
		for ( i = 0; i < groupListSize; i++ )
		{
			MP4SampletoGroupAtomPtr theGroup;
			err = MP4GetListEntry( self->groupList, i, (char **) &theGroup ); if (err) goto bail;
			if ( theGroup ) {
				u32 j;
				for (j=0; j<theGroup->sampleCount; j++) {
					s32 position;
					position = j - total_samples;
					err = mdia->mapSamplestoGroup( mdia, theGroup->grouping_type, (theGroup->group_index)[j], position, 1 );
					if (err) goto bail;
				}
			}
		}
	}

bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err calculateDataSize( MP4TrackFragmentAtomPtr self, u32* outSize )
{
	MP4Err err;
	MP4TrackFragmentHeaderAtomPtr tfhd;
	u32 i;
	u32 totalsize;
	
	totalsize = 0;

	err = MP4NoErr;

	tfhd = (MP4TrackFragmentHeaderAtomPtr) self->tfhd;
	
	if ( self->atomList )
	{
		u32 atomListSize;
		err = MP4GetListEntryCount( self->atomList, &atomListSize ); if (err) goto bail;
		for ( i = 0; i < atomListSize; i++ )
		{
			MP4TrackRunAtomPtr trun;
			err = MP4GetListEntry( self->atomList, i, (char **) &trun ); if (err) goto bail;
			if ( trun ) {
				u32 count;
				u32 j;
				MP4TrackRunEntryPtr entry;
				
				count=trun->samplecount;
				entry = trun->entries;
				
				if (trun->flags & trun_sample_size_present)
				{
					for (j=0; j<count; j++, entry++)
						totalsize += entry->sample_size;
				}
				else
				{
					totalsize += (tfhd->default_sample_size)*count;
				}
			}
		}
	}
	*outSize = totalsize;
	
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err mapSamplestoGroup(struct MP4MediaInformationAtom *s, u32 groupType, u32 group_index, s32 sample_index, u32 count )
{
	MP4Err err;
	MP4SampletoGroupAtomPtr theGroup;
	MP4TrackFragmentAtomPtr self;
	u32 fragment_sample_count, atomListSize, i;
		
	self = (MP4TrackFragmentAtomPtr) s;
	
	err = MP4FindGroupAtom( self->groupList, groupType, (MP4AtomPtr*) &theGroup );
	if (!theGroup) {
		err = MP4CreateSampletoGroupAtom( &theGroup ); if (err) goto bail;
		theGroup->grouping_type = groupType;
		err = addAtom( self, (MP4AtomPtr) theGroup ); if (err) goto bail;
	
		fragment_sample_count = 0;
		err = MP4GetListEntryCount( self->atomList, &atomListSize ); if (err) goto bail;
		for ( i = 0; i < atomListSize; i++ )
		{
			MP4TrackRunAtomPtr trun;
			err = MP4GetListEntry( self->atomList, i, (char **) &trun ); if (err) goto bail;
			if ( trun ) fragment_sample_count += trun->samplecount;
		}

		err = theGroup->addSamples( theGroup, fragment_sample_count );
	}
	err = theGroup->mapSamplestoGroup( theGroup, group_index, sample_index, count ); if (err) goto bail;
	
bail:
	TEST_RETURN( err );

	return err;

}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	PARSE_ATOM_LIST(MP4TrackFragmentAtom)
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateTrackFragmentAtom( MP4TrackFragmentAtomPtr *outAtom )
{
	MP4Err err;
	MP4TrackFragmentAtomPtr self;
	
	self = (MP4TrackFragmentAtomPtr) calloc( 1, sizeof(MP4TrackFragmentAtom) );
	TESTMALLOC( self )

	err = MP4CreateBaseAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->type = MP4TrackFragmentAtomType;
	self->name                = "track fragment";
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->destroy             = destroy;
	err = MP4MakeLinkedList( &self->atomList ); if (err) goto bail;
	err = MP4MakeLinkedList( &self->groupList ); if (err) goto bail;
	self->calculateSize         = calculateSize;
	self->serialize             = serialize;
	self->mdatMoved				= mdatMoved;
	self->addSamples			= addSamples;
	self->addSampleReference	= addSampleReference;
	self->mergeRuns				= mergeRuns;
	self->calculateDataSize		= calculateDataSize;
	self->mapSamplestoGroup		= mapSamplestoGroup;

	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
