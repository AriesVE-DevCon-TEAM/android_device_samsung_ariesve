//--------------------------------------------------------------------------
// Program to pull the information out of various types of EXIF digital 
// camera files and show it in a reasonably consistent way
//
// This module handles basic Jpeg file handling
//
// Matthias Wandel
//--------------------------------------------------------------------------
//#define LOG_NDEBUG 0
#define LOG_TAG "JHEAD"
#include <utils/Log.h>
#include "jhead.h"

// Storage for simplified info extracted from file.
ImageInfo_t CameraHALImageInfo;


static Section_t * Sections = NULL;
static int SectionsAllocated;
static int SectionsRead;
static int HaveAll;

// Define the line below to turn on poor man's debugging output
#undef SUPERDEBUG

#ifdef SUPERDEBUG
#define printf ALOGE
#endif



#define PSEUDO_IMAGE_MARKER 0x123; // Extra value.
//--------------------------------------------------------------------------
// Get 16 bits motorola order (always) for jpeg header stuff.
//--------------------------------------------------------------------------
static int Get16m(const void * Short)
{
    return (((uchar *)Short)[0] << 8) | ((uchar *)Short)[1];
}


//--------------------------------------------------------------------------
// Process a COM marker.
// We want to print out the marker contents as legible text;
// we must guard against random junk and varying newline representations.
//--------------------------------------------------------------------------
static void process_COM (const uchar * Data, int length)
{
    int ch;
    char Comment[MAX_COMMENT_SIZE+1];
    int nch;
    int a;

    nch = 0;

    if (length > MAX_COMMENT_SIZE) length = MAX_COMMENT_SIZE; // Truncate if it won't fit in our structure.

    for (a=2;a<length;a++){
        ch = Data[a];

        if (ch == '\r' && Data[a+1] == '\n') continue; // Remove cr followed by lf.

        if (ch >= 32 || ch == '\n' || ch == '\t'){
            Comment[nch++] = (char)ch;
        }else{
            Comment[nch++] = '?';
        }
    }

    Comment[nch] = '\0'; // Null terminate

    if (ShowTags){
        printf("COM marker comment: %s\n",Comment);
    }

    strcpy(CameraHALImageInfo.Comments,Comment);
    CameraHALImageInfo.CommentWidchars = 0;
}

 
//--------------------------------------------------------------------------
// Process a SOFn marker.  This is useful for the image dimensions
//--------------------------------------------------------------------------
static void process_SOFn (const uchar * Data, int marker)
{
    int data_precision, num_components;

    data_precision = Data[2];
    CameraHALImageInfo.Height = Get16m(Data+3);
    CameraHALImageInfo.Width = Get16m(Data+5);
    num_components = Data[7];

    if (num_components == 3){
        CameraHALImageInfo.IsColor = 1;
    }else{
        CameraHALImageInfo.IsColor = 0;
    }

    CameraHALImageInfo.Process = marker;

    if (ShowTags){
        printf("JPEG image is %uw * %uh, %d color components, %d bits per sample\n",
                   CameraHALImageInfo.Width, CameraHALImageInfo.Height, num_components, data_precision);
    }
}


//--------------------------------------------------------------------------
// Check sections array to see if it needs to be increased in size.
//--------------------------------------------------------------------------
void CheckSectionsAllocated(void)
{
    if (SectionsRead > SectionsAllocated){
        ErrFatal("allocation screwup");
    }
    if (SectionsRead >= SectionsAllocated){
        SectionsAllocated += SectionsAllocated/2;
        Sections = (Section_t *)realloc(Sections, sizeof(Section_t)*SectionsAllocated);
        if (Sections == NULL){
            ErrFatal("could not allocate data for entire image");
        }
    }
}

//--------------------------------------------------------------------------
// Parse the marker buffer until SOS or EOI is seen;
//--------------------------------------------------------------------------
int ReadJpegSectionsFromBuffer (unsigned char* buffer, unsigned int buffer_size, ReadMode_t ReadMode)
{
    int a;
    unsigned int pos = 0;
    int HaveCom = FALSE;

    if (!buffer) {
        return FALSE;
    }

    if (buffer_size < 1) {
        return FALSE;
    }

    a = (int) buffer[pos++];

    if (a != 0xff || buffer[pos++] != M_SOI){
        return FALSE;
    }

    for(;;){
        int itemlen;
        int marker = 0;
        int ll,lh, got;

        CheckSectionsAllocated();

        for (a=0;a<=16;a++){
            marker = buffer[pos++];
            if (marker != 0xff) break;

            if (a >= 16){
                fprintf(stderr,"too many padding bytes\n");
                return FALSE;
            }
        }

        Sections[SectionsRead].Type = marker;

        // Read the length of the section.
        lh = buffer[pos++];
        ll = buffer[pos++];

        itemlen = (lh << 8) | ll;

        if (itemlen < 2) {
            ALOGE("invalid marker");
            return FALSE;
        }

        Sections[SectionsRead].Size = itemlen;

        if (pos+itemlen-2 > buffer_size) {
           ALOGE("Premature end of file?");
          return FALSE;
        }

        SectionsRead += 1;

        ALOGE("reading marker %d", marker);
        switch(marker){
            case M_EXIF:
                // There can be different section using the same marker.
                if (ReadMode & READ_METADATA){
                    if (memcmp(buffer+pos, "Exif", 4) == 0){
                        process_EXIF(buffer+(pos-2), itemlen);
                        break;
                    }
                }
                break;
            default:
                // Skip any other sections.
                if (ShowTags){
                    ALOGD("Jpeg section marker 0x%02x size %d\n",marker, itemlen);
                }
                break;
        }
        pos += itemlen-2;
    }
    return TRUE;
}

//--------------------------------------------------------------------------
// Discard read data.
//--------------------------------------------------------------------------
void DiscardData(void)
{
    int a;

    memset(&CameraHALImageInfo, 0, sizeof(CameraHALImageInfo));
    SectionsRead = 0;
    HaveAll = 0;
}


//--------------------------------------------------------------------------
// Replace or remove exif thumbnail
//--------------------------------------------------------------------------
int SaveThumbnail(char * ThumbFileName)
{
    FILE * ThumbnailFile;

    if (CameraHALImageInfo.ThumbnailOffset == 0 || CameraHALImageInfo.ThumbnailSize == 0){
        fprintf(stderr,"Image contains no thumbnail\n");
        return FALSE;
    }

    if (strcmp(ThumbFileName, "-") == 0){
        // A filename of '-' indicates thumbnail goes to stdout.
        // This doesn't make much sense under Windows, so this feature is unix only.
        ThumbnailFile = stdout;
    }else{
        ThumbnailFile = fopen(ThumbFileName,"wb");
    }

    if (ThumbnailFile){
        uchar * ThumbnailPointer;
        Section_t * ExifSection;
        ExifSection = FindSection(M_EXIF);
        ThumbnailPointer = ExifSection->Data+CameraHALImageInfo.ThumbnailOffset+8;

        fwrite(ThumbnailPointer, CameraHALImageInfo.ThumbnailSize ,1, ThumbnailFile);
        fclose(ThumbnailFile);
        return TRUE;
    }else{
        // ErrFatal("Could not write thumbnail file");
        ALOGE("Could not write thumbnail file");
        return FALSE;
    }
}

//--------------------------------------------------------------------------
// Replace or remove exif thumbnail
//--------------------------------------------------------------------------
int ReplaceThumbnailFromBuffer(const char * Thumb, int ThumbLen)
{
    int NewExifSize;
    Section_t * ExifSection;
    uchar * ThumbnailPointer;

    if (CameraHALImageInfo.ThumbnailOffset == 0 || CameraHALImageInfo.ThumbnailAtEnd == FALSE){
        if (Thumb == NULL){
            // Delete of nonexistent thumbnail (not even pointers present)
            // No action, no error.
            return FALSE;
        }

        // Adding or removing of thumbnail is not possible - that would require rearranging
        // of the exif header, which is risky, and jhad doesn't know how to do.
        fprintf(stderr,"Image contains no thumbnail to replace - add is not possible\n");
#ifdef SUPERDEBUG
        ALOGE("Image contains no thumbnail to replace - add is not possible\n");
#endif
        return FALSE;
    }

    if (Thumb) {
        if (ThumbLen + CameraHALImageInfo.ThumbnailOffset > 0x10000-20){
	        //ErrFatal("Thumbnail is too large to insert into exif header");
	        ALOGE("Thumbnail is too large to insert into exif header");
	        return FALSE;
        }
    } else {
        if (CameraHALImageInfo.ThumbnailSize == 0){
             return FALSE;
        }

        ThumbLen = 0;
    }

    ExifSection = FindSection(M_EXIF);

    NewExifSize = CameraHALImageInfo.ThumbnailOffset+8+ThumbLen;
    ExifSection->Data = (uchar *)realloc(ExifSection->Data, NewExifSize);

    ThumbnailPointer = ExifSection->Data+CameraHALImageInfo.ThumbnailOffset+8;

    if (Thumb){
        memcpy(ThumbnailPointer, Thumb, ThumbLen);
    }

    CameraHALImageInfo.ThumbnailSize = ThumbLen;

    Put32u(ExifSection->Data+CameraHALImageInfo.ThumbnailSizeOffset+8, ThumbLen);

    ExifSection->Data[0] = (uchar)(NewExifSize >> 8);
    ExifSection->Data[1] = (uchar)NewExifSize;
    ExifSection->Size = NewExifSize;

#ifdef SUPERDEBUG
        ALOGE("ReplaceThumbnail successful thumblen %d", ThumbLen);
#endif
    return TRUE;
}

//--------------------------------------------------------------------------
// Replace or remove exif thumbnail
//--------------------------------------------------------------------------
int ReplaceThumbnail(const char * ThumbFileName)
{
    FILE * ThumbnailFile;
    int ThumbLen, NewExifSize;
    Section_t * ExifSection;
    uchar * ThumbnailPointer;

    if (CameraHALImageInfo.ThumbnailOffset == 0 || CameraHALImageInfo.ThumbnailAtEnd == FALSE){
        if (ThumbFileName == NULL){
            // Delete of nonexistent thumbnail (not even pointers present)
            // No action, no error.
            return FALSE;
        }

        // Adding or removing of thumbnail is not possible - that would require rearranging
        // of the exif header, which is risky, and jhad doesn't know how to do.
        fprintf(stderr,"Image contains no thumbnail to replace - add is not possible\n");
#ifdef SUPERDEBUG
        ALOGE("Image contains no thumbnail to replace - add is not possible\n");
#endif
        return FALSE;
    }

    if (ThumbFileName){
        ThumbnailFile = fopen(ThumbFileName,"rb");

        if (ThumbnailFile == NULL){
	        //ErrFatal("Could not read thumbnail file");
	        ALOGE("Could not read thumbnail file");
            return FALSE;
        }

        // get length
        fseek(ThumbnailFile, 0, SEEK_END);

        ThumbLen = ftell(ThumbnailFile);
        fseek(ThumbnailFile, 0, SEEK_SET);

        if (ThumbLen + CameraHALImageInfo.ThumbnailOffset > 0x10000-20){
	        //ErrFatal("Thumbnail is too large to insert into exif header");
	        ALOGE("Thumbnail is too large to insert into exif header");
	        return FALSE;
        }
    }else{
        if (CameraHALImageInfo.ThumbnailSize == 0){
             return FALSE;
        }

        ThumbLen = 0;
        ThumbnailFile = NULL;
    }

    ExifSection = FindSection(M_EXIF);

    NewExifSize = CameraHALImageInfo.ThumbnailOffset+8+ThumbLen;
    ExifSection->Data = (uchar *)realloc(ExifSection->Data, NewExifSize);

    ThumbnailPointer = ExifSection->Data+CameraHALImageInfo.ThumbnailOffset+8;

    if (ThumbnailFile){
        fread(ThumbnailPointer, ThumbLen, 1, ThumbnailFile);
        fclose(ThumbnailFile);
    }

    CameraHALImageInfo.ThumbnailSize = ThumbLen;

    Put32u(ExifSection->Data+CameraHALImageInfo.ThumbnailSizeOffset+8, ThumbLen);

    ExifSection->Data[0] = (uchar)(NewExifSize >> 8);
    ExifSection->Data[1] = (uchar)NewExifSize;
    ExifSection->Size = NewExifSize;

#ifdef SUPERDEBUG
        ALOGE("ReplaceThumbnail successful thumblen %d", ThumbLen);
#endif
    return TRUE;
}


//--------------------------------------------------------------------------
// Discard everything but the exif and comment sections.
//--------------------------------------------------------------------------
void DiscardAllButExif(void)
{
    Section_t ExifKeeper;
    Section_t CommentKeeper;
    Section_t IptcKeeper;
    Section_t XmpKeeper;
    int a;

    memset(&ExifKeeper, 0, sizeof(ExifKeeper));
    memset(&CommentKeeper, 0, sizeof(CommentKeeper));
    memset(&IptcKeeper, 0, sizeof(IptcKeeper));
    memset(&XmpKeeper, 0, sizeof(IptcKeeper));

    for (a=0;a<SectionsRead;a++){
        if (Sections[a].Type == M_EXIF && ExifKeeper.Type == 0){
           ExifKeeper = Sections[a];
        }else if (Sections[a].Type == M_XMP && XmpKeeper.Type == 0){
           XmpKeeper = Sections[a];
        }else if (Sections[a].Type == M_COM && CommentKeeper.Type == 0){
            CommentKeeper = Sections[a];
        }else if (Sections[a].Type == M_IPTC && IptcKeeper.Type == 0){
            IptcKeeper = Sections[a];
        }else{
            free(Sections[a].Data);
        }
    }
    SectionsRead = 0;
    if (ExifKeeper.Type){
        CheckSectionsAllocated();
        Sections[SectionsRead++] = ExifKeeper;
    }
    if (CommentKeeper.Type){
        CheckSectionsAllocated();
        Sections[SectionsRead++] = CommentKeeper;
    }
    if (IptcKeeper.Type){
        CheckSectionsAllocated();
        Sections[SectionsRead++] = IptcKeeper;
    }

    if (XmpKeeper.Type){
        CheckSectionsAllocated();
        Sections[SectionsRead++] = XmpKeeper;
    }
}    

//--------------------------------------------------------------------------
// Write image data back to disk.
//--------------------------------------------------------------------------
int WriteJpegFile(const char * FileName)
{
    FILE * outfile;
    int a;

    if (!HaveAll){
        ALOGE("Can't write back - didn't read all");
        return FALSE;
    }

    outfile = fopen(FileName,"wb");
    if (outfile == NULL){
        ALOGE("Could not open file for write");
        return FALSE;
    }

    // Initial static jpeg marker.
    fputc(0xff,outfile);
    fputc(0xd8,outfile);
    
    if (Sections[0].Type != M_EXIF && Sections[0].Type != M_JFIF){
        // The image must start with an exif or jfif marker.  If we threw those away, create one.
        static uchar JfifHead[18] = {
            0xff, M_JFIF,
            0x00, 0x10, 'J' , 'F' , 'I' , 'F' , 0x00, 0x01, 
            0x01, 0x01, 0x01, 0x2C, 0x01, 0x2C, 0x00, 0x00 
        };
        fwrite(JfifHead, 18, 1, outfile);
    }

    int writeOk = FALSE;
    int nWrite = 0;
    // Write all the misc sections
    for (a=0;a<SectionsRead-1;a++){
        fputc(0xff,outfile);
        fputc((unsigned char)Sections[a].Type, outfile);
	nWrite = fwrite(Sections[a].Data, 1, Sections[a].Size, outfile);
        writeOk = (nWrite == Sections[a].Size);
        if(!writeOk){
            ALOGE("write section %d failed expect %d actual %d",a,Sections[a].Size,nWrite);
            break;
        }
    }

    // Write the remaining image data.
    if (writeOk){
        nWrite = fwrite(Sections[a].Data, 1,Sections[a].Size, outfile);
	writeOk = (nWrite == Sections[a].Size);
        if (!writeOk){
            ALOGE("write section %d failed expect %d actual %d",a,Sections[a].Size,nWrite);
        }
    }
       
    fclose(outfile);
    return writeOk;
}

//--------------------------------------------------------------------------
// Write image to a buffer
//--------------------------------------------------------------------------
int WriteJpegToBuffer(unsigned char* buffer, unsigned int buffer_size)
{
    unsigned int pos = 0;
    int a;

    if (!buffer) {
        return FALSE;
    }

    if (buffer_size < 1) {
        return FALSE;
    }

    if (!HaveAll){
        ALOGE("Can't write back - didn't read all");
        return FALSE;
    }

    // Initial static jpeg marker.
    buffer[pos++] = 0xff;
    buffer[pos++] = 0xd8;

    if (Sections[0].Type != M_EXIF && Sections[0].Type != M_JFIF){
        // The image must start with an exif or jfif marker.  If we threw those away, create one.
        static uchar JfifHead[18] = {
            0xff, M_JFIF,
            0x00, 0x10, 'J' , 'F' , 'I' , 'F' , 0x00, 0x01,
            0x01, 0x01, 0x01, 0x2C, 0x01, 0x2C, 0x00, 0x00
        };
        memcpy(buffer+pos, JfifHead, 18);
        pos+= 18;
    }

    int writeOk = FALSE;
    int nWrite = 0;
    // Write all the misc sections
    for (a=0;a<SectionsRead-1;a++){
        buffer[pos++] = 0xff;
        buffer[pos++] = (unsigned char) Sections[a].Type;
        if (pos+Sections[a].Size > buffer_size) {
            writeOk = FALSE;
            break;
        }
        memcpy(buffer+pos, Sections[a].Data, Sections[a].Size);
        pos += Sections[a].Size;
        writeOk = TRUE;
    }

    // Write the remaining image data.
    if (writeOk){
        if (pos+Sections[a].Size > buffer_size) {
            writeOk = FALSE;
        } else {
            memcpy(buffer+pos, Sections[a].Data, Sections[a].Size);
            pos += Sections[a].Size;
            writeOk = TRUE;
        }
    }
    return writeOk;
}


//--------------------------------------------------------------------------
// Check if image has exif header.
//--------------------------------------------------------------------------
Section_t * FindSection(int SectionType)
{
    int a;

    for (a=0;a<SectionsRead;a++){
        if (Sections[a].Type == SectionType){
            return &Sections[a];
        }
    }
    // Could not be found.
    return NULL;
}

//--------------------------------------------------------------------------
// Remove a certain type of section.
//--------------------------------------------------------------------------
int RemoveSectionType(int SectionType)
{
    int a;
    for (a=0;a<SectionsRead-1;a++){
        if (Sections[a].Type == SectionType){
            // Free up this section
            free (Sections[a].Data);
            // Move succeding sections back by one to close space in array.
            memmove(Sections+a, Sections+a+1, sizeof(Section_t) * (SectionsRead-a));
            SectionsRead -= 1;
            return TRUE;
        }
    }
    return FALSE;
}

//--------------------------------------------------------------------------
// Remove sectons not part of image and not exif or comment sections.
//--------------------------------------------------------------------------
int RemoveUnknownSections(void)
{
    int a;
    int Modified = FALSE;
    for (a=0;a<SectionsRead-1;){
        switch(Sections[a].Type){
            case  M_SOF0:
            case  M_SOF1:
            case  M_SOF2:
            case  M_SOF3:
            case  M_SOF5:
            case  M_SOF6:
            case  M_SOF7:
            case  M_SOF9:
            case  M_SOF10:
            case  M_SOF11:
            case  M_SOF13:
            case  M_SOF14:
            case  M_SOF15:
            case  M_SOI:
            case  M_EOI:
            case  M_SOS:
            case  M_JFIF:
            case  M_EXIF:
            case  M_XMP:
            case  M_COM:
            case  M_DQT:
            case  M_DHT:
            case  M_DRI:
            case  M_IPTC:
                // keep.
                a++;
                break;
            default:
                // Unknown.  Delete.
                free (Sections[a].Data);
                // Move succeding sections back by one to close space in array.
                memmove(Sections+a, Sections+a+1, sizeof(Section_t) * (SectionsRead-a));
                SectionsRead -= 1;
                Modified = TRUE;
        }
    }
    return Modified;
}

//--------------------------------------------------------------------------
// Add a section (assume it doesn't already exist) - used for 
// adding comment sections and exif sections
//--------------------------------------------------------------------------
Section_t * CreateSection(int SectionType, unsigned char * Data, int Size)
{
    Section_t * NewSection;
    int a;
    int NewIndex;
    NewIndex = 2;

    if (SectionType == M_EXIF) NewIndex = 0; // Exif alwas goes first!

    // Insert it in third position - seems like a safe place to put 
    // things like comments.

    if (SectionsRead < NewIndex){
        // ErrFatal("Too few sections!");
        ALOGE("Too few sections!");
        return FALSE;
    }

    CheckSectionsAllocated();
    for (a=SectionsRead;a>NewIndex;a--){
        Sections[a] = Sections[a-1];          
    }
    SectionsRead += 1;

    NewSection = Sections+NewIndex;

    NewSection->Type = SectionType;
    NewSection->Size = Size;
    NewSection->Data = Data;

    return NewSection;
}


//--------------------------------------------------------------------------
// Initialisation.
//--------------------------------------------------------------------------
void ResetJpgfile(void)
{
    if (Sections == NULL){
        Sections = (Section_t *)malloc(sizeof(Section_t)*5);
        SectionsAllocated = 5;
    }

    SectionsRead = 0;
    HaveAll = 0;
}
