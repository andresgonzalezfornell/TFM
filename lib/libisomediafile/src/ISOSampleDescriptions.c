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
derivative works. Copyright (c) 1999, 2000.
*/
/*
	$Id: MP4Media.c,v 1.1.1.1 2002/09/20 08:53:35 julien Exp $
*/
#include "MP4Movies.h"
#include "MP4Atoms.h"
#include "MP4Descriptors.h"
#include "MP4DataHandler.h"
#include <stdlib.h>
#include <string.h>

MP4Err MP4GetMediaESD( MP4Media theMedia, u32 index, MP4ES_DescriptorPtr *outESD, u32 *outDataReferenceIndex );
MP4Err MP4GetMediaSampleDescIndex( MP4Media theMedia, u64 desiredTime, u32 *outSampleDescriptionIndex );

MP4Err MP4CreateMemoryInputStream( char *base, u32 size, MP4InputStreamPtr *outStream );
MP4Err MP4ParseAtomUsingProtoList( MP4InputStreamPtr inputStream, u32* protoList, u32 defaultAtom, MP4AtomPtr *outAtom  );


#ifdef ISMACrypt

u32 MP4SampleEntryProtos[] = { MP4MPEGSampleEntryAtomType, MP4VisualSampleEntryAtomType, MP4AudioSampleEntryAtomType, 
		MP4EncAudioSampleEntryAtomType, MP4EncVisualSampleEntryAtomType, 
		MP4XMLMetaSampleEntryAtomType, MP4TextMetaSampleEntryAtomType, 
		MP4AMRSampleEntryAtomType, MP4AWBSampleEntryAtomType, MP4AMRWPSampleEntryAtomType, MP4H263SampleEntryAtomType, 
		0 };

#else

u32 MP4SampleEntryProtos[] = { MP4MPEGSampleEntryAtomType, MP4VisualSampleEntryAtomType, MP4AudioSampleEntryAtomType, 
		MP4XMLMetaSampleEntryAtomType, MP4TextMetaSampleEntryAtomType, 
		MP4AMRSampleEntryAtomType, MP4AWBSampleEntryAtomType, MP4AMRWPSampleEntryAtomType, MP4H263SampleEntryAtomType, 
		0 };

#endif

MP4Err sampleEntryHToAtomPtr( MP4Handle sampleEntryH, MP4AtomPtr* entryPtr, u32 defaultType ) 
{
	MP4Err err = MP4NoErr;
	MP4InputStreamPtr is;
	u32 size;

	is = NULL;
	
	if ( sampleEntryH == NULL ) return MP4BadParamErr;

	err = MP4GetHandleSize( sampleEntryH, &size ); if (err) goto bail;
	err = MP4CreateMemoryInputStream(*sampleEntryH, size, &is ); if (err) goto bail;
	is->debugging = 0;
	err = MP4ParseAtomUsingProtoList( is, MP4SampleEntryProtos, defaultType, entryPtr ); if (err) goto bail;
	
bail:
	if (is)
		is->destroy(is);
	TEST_RETURN( err );
   	
	return err;
}

MP4Err atomPtrToSampleEntryH( MP4Handle sampleEntryH, MP4AtomPtr entry )
{
	MP4Err err = MP4NoErr;

	err = entry->calculateSize( (MP4AtomPtr) entry ); if (err) goto bail;
	err = MP4SetHandleSize( sampleEntryH, entry->size ); if (err) goto bail;
	err = entry->serialize( (MP4AtomPtr) entry, *sampleEntryH ); if (err) goto bail;

bail:
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) MP4NewSampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
                                                u32 objectTypeIndication,
                                                u32 streamType,
                                                u32 decoderBufferSize,
                                                u32 maxBitrate,
                                                u32 avgBitrate,
                                                MP4Handle decoderSpecificInfoH )
{
    return MP4NewSampleDescriptionWithOCRAssociation( theTrack, sampleDescriptionH,
                                                dataReferenceIndex,
                                                objectTypeIndication,
                                                streamType,
                                                decoderBufferSize,
                                                maxBitrate,
                                                avgBitrate,
                                                decoderSpecificInfoH,
                                                0 );
}

MP4_EXTERN ( MP4Err ) MP4NewSampleDescriptionWithOCRAssociation(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
                                                u32 objectTypeIndication,
                                                u32 streamType,
                                                u32 decoderBufferSize,
                                                u32 maxBitrate,
                                                u32 avgBitrate,
                                                MP4Handle decoderSpecificInfoH,
                                                u32 theOCRESID )
{
   MP4Err MP4CreateMPEGSampleEntryAtom( MP4MPEGSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateVisualSampleEntryAtom( MP4VisualSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateAudioSampleEntryAtom( MP4AudioSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateESDAtom( MP4ESDAtomPtr *outAtom );
   MP4Err MP4CreateES_Descriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );
   MP4Err MP4CreateDecoderConfigDescriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );
   MP4Err MP4CreateSLConfigDescriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );

   MP4Err err;
   GenericSampleEntryAtomPtr     entry;
   MP4ESDAtomPtr                 esdAtom;
   MP4ES_DescriptorPtr           esd;
   MP4DecoderConfigDescriptorPtr config;
   MP4SLConfigDescriptorPtr      slconfig;
   MP4TrackAtomPtr               trak;
   u32 outReferenceIndex;

   if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   trak = (MP4TrackAtomPtr) theTrack;
   if ( trak->newTrackFlags & MP4NewTrackIsVisual )
   {
      err = MP4CreateVisualSampleEntryAtom( (MP4VisualSampleEntryAtomPtr*) &entry ); if (err) goto bail;
   }
   else if ( trak->newTrackFlags & MP4NewTrackIsAudio )
   {
      MP4AudioSampleEntryAtomPtr audioSampleEntry;
      MP4Media media;
      u32 timeScale;

      err = MP4CreateAudioSampleEntryAtom(  (MP4AudioSampleEntryAtomPtr*) &entry ); if (err) goto bail;
      audioSampleEntry = (MP4AudioSampleEntryAtomPtr) entry;
      err = MP4GetTrackMedia( theTrack, &media ); if (err) goto bail;
      err = MP4GetMediaTimeScale( media, &timeScale ); if (err) goto bail;
      audioSampleEntry->timeScale = timeScale;
   }
   else
   {
      err = MP4CreateMPEGSampleEntryAtom( (MP4MPEGSampleEntryAtomPtr*) &entry ); if (err) goto bail;
   }
   entry->dataReferenceIndex = dataReferenceIndex;
   err = MP4CreateESDAtom( &esdAtom ); if (err) goto bail;
   err = MP4CreateES_Descriptor( MP4ES_DescriptorTag, 0, 0, (MP4DescriptorPtr*) &esd ); if (err) goto bail;
   err = MP4CreateDecoderConfigDescriptor( MP4DecoderConfigDescriptorTag, 0, 0,
                                           (MP4DescriptorPtr*) &config ); if (err) goto bail;
   config->objectTypeIndication = objectTypeIndication;
   config->streamType           = streamType;
   config->upstream             = 0;
   config->bufferSizeDB         = decoderBufferSize;
   config->maxBitrate           = maxBitrate;
   config->avgBitrate           = avgBitrate;
   if ( decoderSpecificInfoH )
   {
      MP4Err MP4CreateMemoryInputStream( char *base, u32 size, MP4InputStreamPtr *outStream );
      MP4DescriptorPtr specificInfoDesc;
      MP4InputStreamPtr is;
      u32 infoSize;
                   
      err = MP4GetHandleSize( decoderSpecificInfoH, &infoSize ); if (err) goto bail;
      err = MP4CreateMemoryInputStream( *decoderSpecificInfoH, infoSize, &is ); if (err) goto bail;
      is->debugging = 0;
      err = MP4ParseDescriptor( is, &specificInfoDesc ); if (err) goto bail;
      config->decoderSpecificInfo = specificInfoDesc;
      if (is) {
		is->destroy( is );
			is = NULL;
		}
   }
   esd->decoderConfig = (MP4DescriptorPtr) config;
   err = MP4CreateSLConfigDescriptor( MP4SLConfigDescriptorTag, 0, 0,
                                      (MP4DescriptorPtr*) &slconfig ); if (err) goto bail;
   slconfig->predefined = SLConfigPredefinedMP4;
   slconfig->useTimestampsFlag = 1;

   /* JLF 11/00: OCR is always 0, if OCR wanted, create a sync dependancy */
   esd->OCRESID = 0;
   if (theOCRESID) {
	   /* MP4Err MP4AddTrackReferenceWithID( MP4Track theTrack, u32 dependsOnID, u32 dependencyType, u32 *outReferenceIndex); */
	   /* already declared in MP4Movies.h */
	   err = MP4AddTrackReferenceWithID((MP4Track) trak, theOCRESID, MP4SyncTrackReferenceAtomType, &outReferenceIndex);
	   if (err) goto bail;
   }
   esd->slConfig = (MP4DescriptorPtr) slconfig;
   esdAtom->descriptor =  (MP4DescriptorPtr) esd;
   err = MP4AddListEntry( (void*) esdAtom, entry->ExtensionAtomList ); if (err) goto bail;
   err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;
   
bail:
   if ( esdAtom ) {
		esdAtom->destroy( (MP4AtomPtr) esdAtom );
		/* JLF 07/01: need to set the esd atom to NULL as the SampleDescEntry destructors
		   attempts to delete it */
		if (entry) err = MP4DeleteListEntryAtom( entry->ExtensionAtomList, MP4ESDAtomType );
   }
   		
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

/* JLF: set the stream priority in the sample desc */
MP4_EXTERN ( MP4Err ) MP4SetSampleDescriptionPriority(MP4Handle sampleEntryH, u32 priority)
{
	MP4Err err = MP4NoErr;
	MP4ESDAtomPtr esda;
	MP4ES_Descriptor *esd;
	GenericSampleEntryAtomPtr entry = NULL;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;
	
	err = MP4GetListEntryAtom( entry->ExtensionAtomList, MP4ESDAtomType, (MP4AtomPtr*) &esda ); 
	if ( err == MP4NotFoundErr )
	{
	  BAILWITHERROR( MP4BadParamErr );
	}
	esd = (MP4ES_DescriptorPtr) esda->descriptor;
	if (esd == NULL) {
		err = MP4InvalidMediaErr;
		goto bail;
	}
	esd->streamPriority = priority;

	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}


/* JLF: add a Descriptor to the esd in the sample desc */
MP4_EXTERN ( MP4Err ) MP4AddDescToSampleDescription(MP4Handle sampleEntryH, MP4Handle descriptorH)
{
	MP4Err err = MP4NoErr;
	MP4ESDAtomPtr esda;
	MP4ES_Descriptor *esd;
	MP4DescriptorPtr desc;
	GenericSampleEntryAtomPtr entry = NULL;
	MP4InputStreamPtr is;
	u32 size;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;

	err = MP4GetListEntryAtom( entry->ExtensionAtomList, MP4ESDAtomType, (MP4AtomPtr*) &esda ); 
	if ( err == MP4NotFoundErr )
	{
	  BAILWITHERROR( MP4BadParamErr );
	}
	esd = (MP4ES_DescriptorPtr) esda->descriptor;
	if (esd == NULL) {
		err = MP4InvalidMediaErr;
		goto bail;
	}
	/* then parse our descriptor... */
	err = MP4GetHandleSize( descriptorH, &size ); if (err) goto bail;
	err = MP4CreateMemoryInputStream(*descriptorH, size, &is ); if (err) goto bail;
	is->debugging = 0;
	err = MP4ParseDescriptor( is, (MP4DescriptorPtr *)&desc); if (err) goto bail;
	is->destroy(is);
	
	if (!desc) {
		err = MP4BadParamErr;
		goto bail;
	}
	err = esd->addDescriptor((MP4DescriptorPtr) esd, desc);
	if (err) goto bail;

	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry) entry->destroy((MP4AtomPtr) entry);
	return err;

}

MP4_EXTERN ( MP4Err ) ISOSetSampleDescriptionDimensions(MP4Handle sampleEntryH, u16 width, u16 height)
{
	MP4Err err = MP4NoErr;
	MP4VisualSampleEntryAtomPtr entry = NULL;

	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4VisualSampleEntryAtomType ); if (err) goto bail;
	
	entry->width = width;
	entry->height = height;

	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOSetSampleDescriptionType(MP4Handle sampleEntryH, u32 type)
{
	MP4Err err = MP4NoErr;
	MP4AtomPtr esds;
	MP4VisualSampleEntryAtomPtr entry = NULL;

	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;

	entry->type = type;
	
	type = (type & 0xFFFFFF00) | ' ';
	
	if (type!=MP4_FOUR_CHAR_CODE( 'm', 'p', '4', ' ' )) {
		err = MP4GetListEntryAtom(entry->ExtensionAtomList, MP4ESDAtomType, &esds); if (err) goto bail;
		if (esds) esds->destroy( esds );
		err = MP4DeleteListEntryAtom(entry->ExtensionAtomList, MP4ESDAtomType); if (err) goto bail;
	}
	/* DO NOT set the type of a sample description away from and back to MP4, as you'll lose the esds */

	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOGetSampleDescriptionDimensions(MP4Handle sampleEntryH, u16 *width, u16 *height)
{
	MP4Err err = MP4NoErr;
	MP4VisualSampleEntryAtomPtr entry = NULL;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4VisualSampleEntryAtomType ); if (err) goto bail;

	*width  = entry->width;
	*height = entry->height;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOGetSampleDescriptionType(MP4Handle sampleEntryH, u32 *type)
{
	MP4Err err = MP4NoErr;
	MP4VisualSampleEntryAtomPtr entry = NULL;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;

	*type = entry->type;
	
bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISONewGeneralSampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
                                                u32 sampleEntryType,
                                                MP4GenericAtom extensionAtom )
{
   MP4Err MP4CreateMPEGSampleEntryAtom( MP4MPEGSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateVisualSampleEntryAtom( MP4VisualSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateAudioSampleEntryAtom( MP4AudioSampleEntryAtomPtr *outAtom );

   MP4Err err;
   GenericSampleEntryAtomPtr     entry;
   MP4TrackAtomPtr               trak;

   if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   trak = (MP4TrackAtomPtr) theTrack;
   if ( trak->newTrackFlags & MP4NewTrackIsVisual )
   {
      err = MP4CreateVisualSampleEntryAtom( (MP4VisualSampleEntryAtomPtr*) &entry ); if (err) goto bail;
   }
   else if ( trak->newTrackFlags & MP4NewTrackIsAudio )
   {
      MP4AudioSampleEntryAtomPtr audioSampleEntry;
      MP4Media media;
      u32 timeScale;

      err = MP4CreateAudioSampleEntryAtom(  (MP4AudioSampleEntryAtomPtr*) &entry ); if (err) goto bail;
      audioSampleEntry = (MP4AudioSampleEntryAtomPtr) entry;
      err = MP4GetTrackMedia( theTrack, &media ); if (err) goto bail;
      err = MP4GetMediaTimeScale( media, &timeScale ); if (err) goto bail;
      audioSampleEntry->timeScale = timeScale;
   }
   else
   {
	/* this case covers MP4NewTrackIsMetadata */
      err = MP4CreateMPEGSampleEntryAtom( (MP4MPEGSampleEntryAtomPtr*) &entry ); if (err) goto bail;
   }
   entry->dataReferenceIndex = dataReferenceIndex;
   entry->type = sampleEntryType;

   if ( extensionAtom ) {
      err = MP4AddListEntry( (void*) extensionAtom, entry->ExtensionAtomList); if (err) goto bail;
	}

	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) ISOAddAtomToSampleDescription(MP4Handle sampleEntryH, MP4GenericAtom extensionAtom)
{
	MP4Err err = MP4NoErr;
	MP4GenericSampleEntryAtomPtr entry = NULL;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;

   if ( extensionAtom ) {
		err = MP4AddListEntry( (void*) extensionAtom, entry->ExtensionAtomList); if (err) goto bail;
	}
	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOGetAtomFromSampleDescription(MP4Handle sampleEntryH, u32 atomType, MP4GenericAtom* outAtom)
{
	MP4Err err = MP4NoErr;
	MP4GenericSampleEntryAtomPtr entry = NULL;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;
	err = MP4GetListEntryAtom(entry->ExtensionAtomList, atomType, (MP4AtomPtr*) outAtom );  if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOAddBitrateToSampleDescription(MP4Handle sampleEntryH, u8 is_3GPP, u32 buffersizeDB, u32 maxBitrate, u32 avgBitrate)
{
	MP4Err err = MP4NoErr;
	MP4GenericSampleEntryAtomPtr entry = NULL;
	MP4BitRateAtomPtr rate;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;
	
	err = MP4CreateBitRateAtom( &rate ); if (err) goto bail;
	rate->type = (is_3GPP ? TGPPBitRateAtomType : MP4BitRateAtomType );
	rate->buffersizeDB = buffersizeDB;
	rate->max_bitrate = maxBitrate;
	rate->avg_bitrate = avgBitrate;

	err = MP4AddListEntry( (void*) rate, entry->ExtensionAtomList); if (err) goto bail;

	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISONewXMLMetaDataSampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
												char*	content_encoding,
												char*	xml_namespace,
												char*	schema_location )
{
   MP4Err err;
   MP4XMLMetaSampleEntryAtomPtr     entry;
   MP4TrackAtomPtr               trak;

   if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   trak = (MP4TrackAtomPtr) theTrack;
   err = MP4CreateXMLMetaSampleEntryAtom( (MP4XMLMetaSampleEntryAtomPtr*) &entry ); if (err) goto bail;

   entry->dataReferenceIndex = dataReferenceIndex;

	if (content_encoding) { 
		entry->content_encoding = calloc( strlen(content_encoding)+1, 1 );
		TESTMALLOC( entry->content_encoding );
		strcpy( entry->content_encoding, content_encoding );
	} else entry->content_encoding = NULL;
	if (xml_namespace) { 
		entry->xml_namespace = calloc( strlen(xml_namespace)+1, 1 );
		TESTMALLOC( entry->xml_namespace );
		strcpy( entry->xml_namespace, xml_namespace );
	} else entry->xml_namespace = NULL;
	if (schema_location) { 
		entry->schema_location = calloc( strlen(schema_location)+1, 1 );
		TESTMALLOC( entry->schema_location );
		strcpy( entry->schema_location, schema_location );
	} else entry->schema_location = NULL;

	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) ISONewTextMetaDataSampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
												char*	content_encoding,
												char*	mime_format )
{
   MP4Err err;
   MP4TextMetaSampleEntryAtomPtr     entry;
   MP4TrackAtomPtr               trak;

   if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   trak = (MP4TrackAtomPtr) theTrack;
   err = MP4CreateTextMetaSampleEntryAtom( (MP4TextMetaSampleEntryAtomPtr*) &entry ); if (err) goto bail;

   entry->dataReferenceIndex = dataReferenceIndex;

	if (content_encoding) { 
		entry->content_encoding = calloc( strlen(content_encoding)+1, 1 );
		TESTMALLOC( entry->content_encoding );
		strcpy( entry->content_encoding, content_encoding );
	} else entry->content_encoding = NULL;
	if (mime_format) { 
		entry->mime_format = calloc( strlen(mime_format)+1, 1 );
		TESTMALLOC( entry->mime_format );
		strcpy( entry->mime_format, mime_format );
	} else entry->mime_format = NULL;

	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) ISONewH263SampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
												u32 vendor,
												u8 decoder_version,
												u8 H263_level,
												u8 H263_profile)
{
	MP4Err err;
	GenericSampleEntryAtomPtr     entry;
	MP4H263SpecificInfoAtomPtr	  H263SpecificInfoAtom;
	MP4TrackAtomPtr               trak;

	if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
	{
	  BAILWITHERROR( MP4BadParamErr );
	}

	trak = (MP4TrackAtomPtr) theTrack;
	if (!( trak->newTrackFlags & MP4NewTrackIsVisual )) BAILWITHERROR (MP4BadParamErr );
	err = MP4CreateVisualSampleEntryAtom( (MP4VisualSampleEntryAtomPtr*) &entry ); if (err) goto bail;

	entry->dataReferenceIndex = dataReferenceIndex;
	entry->type = MP4H263SampleEntryAtomType;

	err = MP4CreateH263SpecificInfoAtom( &H263SpecificInfoAtom ); if (err) goto bail;
	H263SpecificInfoAtom->vendor             = vendor;
	H263SpecificInfoAtom->decoder_version    = decoder_version;
	H263SpecificInfoAtom->H263_level         = H263_level;
	H263SpecificInfoAtom->H263_profile       = H263_profile;
   
	err = MP4AddListEntry( (void*) H263SpecificInfoAtom, entry->ExtensionAtomList); if (err) goto bail;

	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) ISONewAMRSampleDescription(  MP4Track theTrack,
						MP4Handle sampleDescriptionH,
                        u32 dataReferenceIndex,
						u8 is_WB,
						u32 vendor,
						u8 decoder_version,
						u16 mode_set,
						u8 mode_change_period,
						u8 frames_per_sample)
{
	MP4Err err;
	MP4AudioSampleEntryAtomPtr     entry;
	MP4AMRSpecificInfoAtomPtr	   AMRSpecificInfoAtom;
	MP4TrackAtomPtr               trak;
	MP4Media media;
	u32 timeScale;
	  
	if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
	{
	  BAILWITHERROR( MP4BadParamErr );
	}

	trak = (MP4TrackAtomPtr) theTrack;
	if (!( trak->newTrackFlags & MP4NewTrackIsAudio )) BAILWITHERROR (MP4BadParamErr );
	err = MP4CreateAudioSampleEntryAtom( (MP4AudioSampleEntryAtomPtr*) &entry ); if (err) goto bail;

	err = MP4GetTrackMedia( theTrack, &media ); if (err) goto bail;
	err = MP4GetMediaTimeScale( media, &timeScale ); if (err) goto bail;
	entry->timeScale = timeScale;

	entry->dataReferenceIndex = dataReferenceIndex;
	entry->type = (is_WB ? MP4AWBSampleEntryAtomType : MP4AMRSampleEntryAtomType);

	err = MP4CreateAMRSpecificInfoAtom( &AMRSpecificInfoAtom ); if (err) goto bail;
	AMRSpecificInfoAtom->vendor					= vendor;
	AMRSpecificInfoAtom->decoder_version		= decoder_version;
	AMRSpecificInfoAtom->mode_set				= mode_set;
	AMRSpecificInfoAtom->mode_change_period		= mode_change_period;
	AMRSpecificInfoAtom->frames_per_sample		= frames_per_sample;
	   
	err = MP4AddListEntry( (void*) AMRSpecificInfoAtom, entry->ExtensionAtomList); if (err) goto bail;

	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) ISONewAMRWPSampleDescription(  MP4Track theTrack,
						MP4Handle sampleDescriptionH,
						u32 dataReferenceIndex,
						u32 vendor,
						u8 decoder_version)
{
	MP4Err err;
	MP4AudioSampleEntryAtomPtr     entry;
	MP4AMRWPSpecificInfoAtomPtr	   AMRSpecificInfoAtom;
	MP4TrackAtomPtr                trak;
	MP4Media media;
	u32 timeScale;
	  
	if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
	{
	  BAILWITHERROR( MP4BadParamErr );
	}

	trak = (MP4TrackAtomPtr) theTrack;
	if (!( trak->newTrackFlags & MP4NewTrackIsAudio )) BAILWITHERROR (MP4BadParamErr );
	err = MP4CreateAudioSampleEntryAtom( (MP4AudioSampleEntryAtomPtr*) &entry ); if (err) goto bail;

	err = MP4GetTrackMedia( theTrack, &media ); if (err) goto bail;
	err = MP4GetMediaTimeScale( media, &timeScale ); if (err) goto bail;
	entry->timeScale = timeScale;

	entry->dataReferenceIndex = dataReferenceIndex;
	entry->type = MP4AMRWPSampleEntryAtomType;

	err = MP4CreateAMRWPSpecificInfoAtom( &AMRSpecificInfoAtom ); if (err) goto bail;
	AMRSpecificInfoAtom->vendor					= vendor;
	AMRSpecificInfoAtom->decoder_version		= decoder_version;
	   
	err = MP4AddListEntry( (void*) AMRSpecificInfoAtom, entry->ExtensionAtomList); if (err) goto bail;

	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

/* whole loads of bitBuffer stuff for parsing parameter sets */

typedef struct {
	u8 *ptr;
	u32 length;
	u8 *cptr;
	u8 cbyte;
	u32 curbits;
	u32 bits_left;
	
	u8 prevent_emulation;	/* true or false */
	u8 emulation_position;	/* 0 usually, 1 after 1 zero byte, 2 after 2 zero bytes, 
								 3 after 00 00 03, and the 3 gets stripped */
} BitBuffer;

static MP4Err BitBuffer_Init(BitBuffer *bb, u8 *p, u32 length)
{
	int err = MP4NoErr;
	
	if (length > 0x0fffffff) {
		err = MP4BadParamErr;
		goto bail;
	}

	bb->ptr = (void*)p;
	bb->length = length;
	
	bb->cptr = (void*)p;
	bb->cbyte = *bb->cptr;
	bb->curbits = 8;
	
	bb->bits_left = length * 8;
	
	bb->prevent_emulation = 0;
	bb->emulation_position = (bb->cbyte == 0 ? 1 : 0);
	
bail:
	return err;
}


static u32 GetBits(BitBuffer *bb, u32 nBits, MP4Err *errout)
{
	MP4Err err = MP4NoErr;
	int myBits;
	int myValue;
	int myResidualBits;
	int leftToRead;
	
	myValue = 0;
	if (nBits>bb->bits_left) {
		err = MP4EOF;
		goto bail;
	}
	
	if (bb->curbits <= 0) {
		bb->cbyte = *++bb->cptr;
		bb->curbits = 8;
		
		if (bb->prevent_emulation != 0) {
			if ((bb->emulation_position >= 2) && (bb->cbyte == 3)) {
				bb->cbyte = *++bb->cptr;
				bb->bits_left -= 8;
				bb->emulation_position = 0;
				if (nBits>bb->bits_left) {
					err = MP4EOF;
					goto bail;
				}
			}
			else if (bb->cbyte == 0) bb->emulation_position += 1;
			else bb->emulation_position = 0;
		}
	}
	
	if (nBits > bb->curbits)
		myBits = bb->curbits;
	else
		myBits = nBits;
		
	myValue = (bb->cbyte>>(8-myBits));
	myResidualBits = bb->curbits - myBits;
	leftToRead = nBits - myBits;
	bb->bits_left -= myBits;
	
	bb->curbits = myResidualBits;
	bb->cbyte = ((bb->cbyte) << myBits) & 0xff;
	
	if (leftToRead > 0) {
		u32 newBits;
		newBits = GetBits(bb, leftToRead, &err);
		myValue = (myValue<<leftToRead) | newBits;
	}
	
bail:	
	if (errout) *errout = err;
	return myValue;
}

static MP4Err GetBytes(BitBuffer *bb, u32 nBytes, u8 *p)
{
	MP4Err err = MP4NoErr;
	unsigned int i;
	
	for (i = 0; i < nBytes; i++) {
		*p++ = (u8)GetBits(bb, 8, &err);
		if (err) break;
	}
	
	return err;
}

static u32 read_golomb_uev(BitBuffer *bb, MP4Err *errout)
{
	MP4Err err = MP4NoErr;
	
	u32 power = 1;
	u32 value = 0;
	u32 leading = 0;
	u32 nbits = 0;
	
	leading = GetBits(bb, 1, &err);  if (err) goto bail;
	
	while (leading == 0) { 
		power = power << 1;
		nbits++;
		leading = GetBits(bb, 1, &err);  if (err) goto bail;
	}
	
	if (nbits > 0) {
		value = GetBits( bb, nbits, &err); if (err) goto bail;
	}
	
bail:
	if (errout) *errout = err;
	return (power - 1 + value);
}

MP4_EXTERN ( MP4Err ) ISONewAVCSampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
												u32 length_size, MP4Handle first_sps, MP4Handle first_pps, MP4Handle first_spsext )
{
   MP4Err MP4CreateVisualSampleEntryAtom( MP4VisualSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateVCConfigAtom( ISOVCConfigAtomPtr *outAtom );
   
   MP4Err err;
   GenericSampleEntryAtomPtr     entry;
   ISOVCConfigAtomPtr			 config;
   MP4TrackAtomPtr               trak;
   BitBuffer mybb;
   BitBuffer *bb;
   
   u32 the_size, y;
   u8 x; 

   if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   trak = (MP4TrackAtomPtr) theTrack;
   if (!( trak->newTrackFlags & MP4NewTrackIsVisual )) BAILWITHERROR (MP4BadParamErr );
   err = MP4CreateVisualSampleEntryAtom( (MP4VisualSampleEntryAtomPtr*) &entry ); if (err) goto bail;

   entry->dataReferenceIndex = dataReferenceIndex;
   entry->type = ISOAVCSampleEntryAtomType;

   err = MP4CreateVCConfigAtom( &config );
   config->length_size = length_size;
   
   err = MP4AddListEntry( (void*) config, entry->ExtensionAtomList); if (err) goto bail;

   if (!first_sps) BAILWITHERROR( MP4BadParamErr );
   err = MP4GetHandleSize( first_sps, &the_size ); if (err) goto bail;
   
   bb = &mybb;
   err = BitBuffer_Init( bb, (u8*) *first_sps, 8*the_size ); if (err) goto bail;
   err = GetBytes( bb, 1, &x ); if (err) goto bail;
   if ((x & 0x1F) != 7) BAILWITHERROR( MP4BadParamErr );
	/* 7 == SPS */
   err = GetBytes( bb, 1, &x ); if (err) goto bail;
   config->profile = x;
   err = GetBytes( bb, 1, &x ); if (err) goto bail;
   config->profile_compatibility = x;
   err = GetBytes( bb, 1, &x ); if (err) goto bail;
   config->level = x;
   y = read_golomb_uev( bb, &err ); if (err) goto bail;	/* PPS ID */
   if( (config->profile  ==  100)  ||  (config->profile  ==  110)  ||
	   (config->profile  ==  122)  ||  (config->profile  ==  144) )
   {
		y = read_golomb_uev( bb, &err ); if (err) goto bail;
		config->chroma_format = y;
		y = read_golomb_uev( bb, &err ); if (err) goto bail;
		config->bit_depth_luma_minus8 = y;
		y = read_golomb_uev( bb, &err ); if (err) goto bail;
		config->bit_depth_chroma_minus8 = y;
   }
   
   if (first_sps && first_pps)    
                     { err = config->addParameterSet( config, first_sps,    AVCsps );    if (err) goto bail; }
   if (first_pps)    { err = config->addParameterSet( config, first_pps,    AVCpps );    if (err) goto bail; }
   if (first_spsext) { err = config->addParameterSet( config, first_spsext, AVCspsext ); if (err) goto bail; }
   
	err = atomPtrToSampleEntryH( sampleDescriptionH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) ISOAddVCSampleDescriptionPS(  MP4Handle sampleEntryH,
                                                MP4Handle ps, u32 where )
{
	MP4Err err = MP4NoErr;
	ISOVCConfigAtomPtr			 config;
	MP4VisualSampleEntryAtomPtr entry = NULL;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4GenericSampleEntryAtomType ); if (err) goto bail;

	if (entry->type != ISOAVCSampleEntryAtomType) BAILWITHERROR( MP4BadParamErr );

	err = MP4GetListEntryAtom( entry->ExtensionAtomList, ISOVCConfigAtomType, (MP4AtomPtr*) &config ); 
	if ( err == MP4NotFoundErr )
	{
	  BAILWITHERROR( MP4BadDataErr );
	}
	
	err = config->addParameterSet( config, ps, where );    if (err) goto bail;
	
	/* rewrite it... */
	err = atomPtrToSampleEntryH( sampleEntryH, (MP4AtomPtr) entry ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOGetAVCSampleDescription(  MP4Handle sampleEntryH,
                                                u32 *dataReferenceIndex,
												u32 *length_size, u32* sps_count, u32* pps_count, u32* spsext_count )
{
	MP4Err err = MP4NoErr;
	MP4VisualSampleEntryAtomPtr entry = NULL;
	ISOVCConfigAtomPtr			 config;
	
	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4VisualSampleEntryAtomType ); if (err) goto bail;

	if (entry->type != ISOAVCSampleEntryAtomType) BAILWITHERROR( MP4BadParamErr );
	err = MP4GetListEntryAtom( entry->ExtensionAtomList, ISOVCConfigAtomType, (MP4AtomPtr*) &config ); 
	if ( err == MP4NotFoundErr )
	{
	  BAILWITHERROR( MP4BadDataErr );
	}
	
	*dataReferenceIndex = entry->dataReferenceIndex;
	*length_size = config->length_size;
	err = MP4GetListEntryCount( config->spsList, sps_count ); if (err) goto bail;
	err = MP4GetListEntryCount( config->ppsList, pps_count ); if (err) goto bail;
	err = MP4GetListEntryCount( config->spsextList, spsext_count ); if (err) goto bail;
	
bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}

MP4_EXTERN ( MP4Err ) ISOGetVCSampleDescriptionPS(  MP4Handle sampleEntryH,
                                                MP4Handle ps, u32 where, u32 index )
{
	MP4Err err = MP4NoErr;
	MP4VisualSampleEntryAtomPtr entry = NULL;
	ISOVCConfigAtomPtr			 config;

	err = sampleEntryHToAtomPtr( sampleEntryH, (MP4AtomPtr *) &entry, MP4VisualSampleEntryAtomType ); if (err) goto bail;
	
	if (entry->type != ISOAVCSampleEntryAtomType) BAILWITHERROR( MP4BadParamErr );
	err = MP4GetListEntryAtom( entry->ExtensionAtomList, ISOVCConfigAtomType, (MP4AtomPtr*) &config ); 
	if ( err == MP4NotFoundErr )
	{
	  BAILWITHERROR( MP4BadDataErr );
	}
	
	err = config->getParameterSet( config, ps, where, index );    if (err) goto bail;
	
bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	return err;
}