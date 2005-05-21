 /***************************************************************************
 *                                                                         *
 *   (c) Art Tevs, MPI Informatik Saarbruecken                             *
 *       mailto: <tevs@mpi-sb.mpg.de>                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 * Original code was written by  Alexander Zaprjagaev <frustum@frustum.org>*
 *                                                                         *
 **************************************************************************/ 
#include "nrCTextureLoader.h"

/*------------------------------------------------------------------------------
 * Other includes
 -----------------------------------------------------------------------------*/
#include <string.h>
#include <png.h>
#include <setjmp.h>
extern "C" {
	#include <jpeglib.h>
}

#ifdef _WIN32
#pragma comment (lib,"libpng.lib")
#pragma comment (lib,"zlib.lib")
#pragma comment (lib,"libjpeg.lib")
#endif


/**
 * swap contant of 2 bytes
 **/
static inline void swap(byte & a, byte & b){
    byte temp;

    temp = a;
    a    = b;
    b    = temp;

    return;
}

/**
 * Initialize the instance
 **/
nrCTextureLoader::nrCTextureLoader(){

}

/** 
 * Release the instacne
 **/	
nrCTextureLoader::~nrCTextureLoader(){

}


/*****************************************************************************/
/*                                                                           */
/* IO functions                                                              */
/*                                                                           */
/*****************************************************************************/

byte range_compress(const float & f)
{ return (byte)((f+1)*127.5); }
	
/**
 * Convert given data into normal map
 * To get height of current pixel the grey scale value will be computed 
 **/
shared_array<byte> nrCTextureLoader::heightmap_to_normalmap (shared_array<byte>& data, int width, int height){

	// define putput
	shared_array<byte> dst (new byte[width*height*4]);
	shared_array<byte> src = rgba2luminance(data,width,height);
	
	// go through all pixels
	for (int x=1; x < width-1; x++){
		for (int y=1; y < height-1; y++){
			// wir lesen zunaechst uns interessierte werte aus
			byte g1 = src[y*width + x+1];
			byte g2 = src[y*width + x-1];
			vec3 dfdx(2.0f, 0.0f, 4.0*float(g1-g2)/255.0f);
			
			// und jetzt das andere
			g1 = src[(y+1)*width + x];
			g2 = src[(y-1)*width + x];
			vec3 dfdy(0.f, 2.0f, 4.0*float(g1 - g2)/255.0f);
			
			// verrechnen und speichern
			vec3 n = cross(dfdx, dfdy);
			n.normalize();
			
			// Ergebnis speichern
			dst[y*4*width + x*4 + 0] = range_compress(n[0]);			
			dst[y*4*width + x*4 + 1] = range_compress(n[1]);			
			dst[y*4*width + x*4 + 2] = range_compress(n[2]);			
			dst[y*4*width + x*4 + 3] = 255;
		}		
	}
	
	// kopiere den Inhalt von den Rändern zu den letzten Pixeln
	for(int i = 0; i < width; i++)
	{
		dst[0*4*width + i*4 + 0] = dst[1*4*width + i*4 + 0];			
		dst[0*4*width + i*4 + 1] = dst[1*4*width + i*4 + 1];			
		dst[0*4*width + i*4 + 2] = dst[1*4*width + i*4 + 2];			
		dst[0*4*width + i*4 + 3] = dst[1*4*width + i*4 + 3];

		dst[(height-1)*4*width + i*4 + 0] = dst[(height-2)*4*width + i*4 + 0];			
		dst[(height-1)*4*width + i*4 + 1] = dst[(height-2)*4*width + i*4 + 1];			
		dst[(height-1)*4*width + i*4 + 2] = dst[(height-2)*4*width + i*4 + 2];			
		dst[(height-1)*4*width + i*4 + 3] = dst[(height-2)*4*width + i*4 + 3];
	}
	for(int j = 0; j < height; j++)
	{
		dst[j*4*width + 0*4 + 0] = dst[j*4*width + 1*4 + 0];			
		dst[j*4*width + 0*4 + 1] = dst[j*4*width + 1*4 + 1];			
		dst[j*4*width + 0*4 + 2] = dst[j*4*width + 1*4 + 2];			
		dst[j*4*width + 0*4 + 3] = dst[j*4*width + 1*4 + 3];

		dst[j*4*width + (width-1)*4 + 0] = dst[j*4*width + (width-2)*4 + 0];			
		dst[j*4*width + (width-1)*4 + 1] = dst[j*4*width + (width-2)*4 + 1];			
		dst[j*4*width + (width-1)*4 + 2] = dst[j*4*width + (width-2)*4 + 2];			
		dst[j*4*width + (width-1)*4 + 3] = dst[j*4*width + (width-2)*4 + 3];
	}
	
	return dst;
}


/**
 * Load texture from file.
 * @param vfs - virtual filesystem where we can load files from
 * @param name - name of the file to be loaded
 * @param width - return width of the file
 * @param height - return height of the file
 * @return smart array containing data of the file
 */
shared_array<byte> nrCTextureLoader::load(nrCFileSystem* vfs, const string& name,int &width,int &height) {
  	
	if(strstr(name.c_str(),".tga") || strstr(name.c_str(),".TGA")) return load_tga(vfs,name,width,height);
	if(strstr(name.c_str(),".png") || strstr(name.c_str(),".PNG")) return load_png(vfs,name,width,height);
	if(strstr(name.c_str(),".jpg") || strstr(name.c_str(),".JPG")) return load_jpeg(vfs,name,width,height);
	if(strstr(name.c_str(),".dds") || strstr(name.c_str(),".DDS")) return load_dds(vfs,name,width,height);
	
	nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load():unknown format of '%s' file",name.c_str());
	return shared_array<byte>(NULL);
}


/**
 * Save texture to file. 
 * @param name - name of the file
 * @param data - smart array containing data of the file
 * @param width - width of the data
 * @param height - height of the data
 * @return 0 if no error occurs
 * @note: Can only be used with 32-Bit per Pixel data (RGBA).
 */
int nrCTextureLoader::save(const string& name,const shared_array<byte>& data,int width,int height) {
	if(strcmp(name.c_str(),".tga")) return save_tga(name,data,width,height);
	if(strcmp(name.c_str(),".jpg")) return save_jpeg(name,data,width,height,80);
	
	nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::save():unknown format of '%s' file",name.c_str());
	return 0;
}



/*****************************************************************************/
/*                                                                           */
/* tga images                                                                */
/*                                                                           */
/*****************************************************************************/

#define TGA_TYPE_MAPPED 1
#define TGA_TYPE_COLOR 2
#define TGA_TYPE_GRAY 3
#define TGA_TYPE_MAPPED_RLE 9
#define TGA_TYPE_COLOR_RLE 10
#define TGA_TYPE_GRAY_RLE 11
#define TGA_DESC_ABITS 0x0f
#define TGA_DESC_HORIZONTAL 0x10
#define TGA_DESC_VERTICAL 0x20


/**
 * Tga-File Header.
 * I do not use this header here because of wrong (probably a bug) gcc.
 * The size of the header must be 18 Bytes and sizeof(_tgaHeader) produce
 * by my gcc-3.3.5 20 Bytes. I do not know why. Probably bug
 */
struct  _tgaHeader{
    char  idlength;
    char  colourmaptype;
    char  datatypecode;
    short colourmaporigin;
    short colourmaplength;
    char  colourmapdepth;
    short x_origin;
    short y_origin;
    short SizeX;
    short SizeY;
    char  Bpp;
    char  imagedescriptor;
};


/**
 * Load tga images. Supports 8,24 adn 32 Bit TGA-Images. Image can be compressed
 * but only in 24,32 Bit mode. Compression of 8 Bit mode is not supported.
 * @param vfs - virtual filesystem from which file will be loaded
 * @param name - file name of the file
 * @param width,height - load_tga() will return here the size of loaded image data
 * @return smart array containing data (always 32-Bit per Pixel)
 */
shared_array<byte> nrCTextureLoader::load_tga(nrCFileSystem* vfs, const string& name,int &width,int &height) {
	
	// open file through our file system
	int	file = 0;	
	nrResult ret = vfs->openFile(&file, name.c_str(), true);
	if(ret != NR_OK) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_tga():error open '%s' file",name.c_str());
		return shared_array<byte>(NULL);
	}
	  
    // Read the header
    byte header[18];
	memset(header, 0, 18);
	ret = vfs->readData(file, 18, header);
	if (ret != NR_OK){
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_tga(): corrupt file '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
				
	// Check bit depth
	int components = 0;
	if (header[16] == 8) components = 1;
	else if(header[16] == 24) components = 3;
	else if(header[16] == 32) components = 4;
	else {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_tga(): only 8,24 and 32 bpp images are suported");
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	
	// calculate size and export data from header
	width = (int)header[12] + ((int)header[13]) * 256;
	height = (int)header[14] + ((int)header[15]) * 256;

	// allocate memory for reading data
	int size = width * height * components;
	shared_array<byte> buf (new byte[size]);
	shared_array<byte> data (new byte[width * height * 4]);
	
	// seek to data
	vfs->seek(file, header[0], SEEK_CUR);
	
	// we have simple colour data
	if(header[2] == TGA_TYPE_COLOR) {	
		vfs->readData(file, size, buf.get());
		
	// we have compressed data
	} else if(header[2] == TGA_TYPE_COLOR_RLE) {
		byte *ptr = buf.get();
		for(int i = 0; i < size;) {
			byte rep;
			vfs->readData(file,1,&rep);
			if(rep & 0x80) {
				rep ^= 0x80;
				vfs->readData(file, components, ptr);
				ptr += components;
				for(int j = 0; j < rep * components; j++) {
					*ptr = *(ptr - components);
					ptr++;
				}
				i += components * (rep + 1);
			} else {
				int j = components * (rep + 1);
				vfs->readData(file, j, ptr); 
				ptr += j;
				i += j;
			}
		}
	} else {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_tga(): Corrupt compression !!!");
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	
	
	// convert bgr to rgb color
	for(int i = 0, j = 0; i < size; i += components, j += 4) {
		if (components == 1){
			data[j] = buf[i];
			data[j + 1] = buf[i];
			data[j + 2] = buf[i];
			data[j + 3] = 255;			
		}else if (components == 3){
			data[j] = buf[i + 2];
			data[j + 1] = buf[i + 1];
			data[j + 2] = buf[i];						
		}else if (components == 4){
			data[j] = buf[i + 2];
			data[j + 1] = buf[i + 1];
			data[j + 2] = buf[i];			
			data[j + 3] = buf[i + 3];
		}
	}
	
	// flip verticaly
	if(!(header[17] & 0x20)) {
		for(int j = 0; j < height / 2; j++) {
			for(int i = 0; i < width * 4; i++) {
				byte c = data[j * width * 4 + i];
				data[j * width * 4 + i] = data[(height - j - 1) * width * 4 + i];
				data[(height - j - 1) * width * 4 + i] = c;
			}
		}
	}
	
	vfs->closeFile(file);
	return data;
}


/**
 * save tga images
 * @param name - file name to which data should be saved
 * @param data - smart array containing data to be stored (must be 32Bit per Pixel)
 * @param width,height - size of the image data
 * @return 0 if no error occurs
 */
int nrCTextureLoader::save_tga(const string& name,const shared_array<byte>& data,int width,int height) {
	
	char nname[1024];
	sprintf(nname, "%s%s", nrVFS.getPathToFileSystem(), name.c_str());
	FILE *file = fopen(nname,"wb");
	if(!file) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::save_tga(): error create '%s' file",name.c_str());
		return 0;
	}
	shared_array<byte> buf(new byte[18 + width * height * 4]);
	memset(buf.get(),0,18);
	buf[2] = 2;
	buf[12] = width % 256;
	buf[13] = width / 256;
	buf[14] = height % 256;
	buf[15] = height / 256;
	buf[16] = 32;
	buf[17] = 0x28;
	memcpy(buf.get() + 18,data.get(),width * height * 4);

	// rgba->bgra
	for(int i = 18; i < 18 + width * height * 4; i += 4) {
		byte c = buf[i];
		buf[i] = buf[i + 2];
		buf[i + 2] = c;
	}
	fwrite(buf.get(),1,18 + width * height * 4,file);
	fclose(file);
	return 1;
}

/*****************************************************************************/
/*                                                                           */
/* jpeg images                                                               */
/*                                                                           */
/*****************************************************************************/

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

static void my_error_exit(j_common_ptr cinfo) {
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}

/*
 * These four functions constitute a simple memory reader, for making
 * libjpeg read from a pointer, not a file
 */
void noop(j_decompress_ptr cinfo) {}
#if defined(__GNUC__) || defined(__ICC__)
int fill(j_decompress_ptr cinfo)
#else
byte fill(j_decompress_ptr cinfo)
#endif
{
	if (cinfo->src->bytes_in_buffer >= 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}
void skip (j_decompress_ptr cinfo, long num_bytes)
{
	cinfo->src->bytes_in_buffer -= num_bytes;
	cinfo->src->next_input_byte += num_bytes;
}

void init_mem_source(j_decompress_ptr cinfo, shared_ptr<nrCFile> file)
{
	cinfo->src = (struct jpeg_source_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
					     sizeof(struct jpeg_source_mgr));

	cinfo->src->init_source = noop;
	cinfo->src->fill_input_buffer = fill;
	cinfo->src->skip_input_data = skip;
	cinfo->src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
	cinfo->src->term_source = noop;
	cinfo->src->bytes_in_buffer = file->dataLength(); /* forces fill_input_buffer on first read */
	cinfo->src->next_input_byte = (const JOCTET *)file->getData(); /* until buffer loaded */
}



/**
 * load jpeg images
 * @param vfs - virtual filesystem from which file will be loaded
 * @param name - file name of the file
 * @param width,height - load_jpeg() will return here the size of loaded image data
 * @return smart array containing data (always 32-Bit per Pixel)
 */
shared_array<byte> nrCTextureLoader::load_jpeg(nrCFileSystem* vfs,const string& name,int &width,int &height) {
	
	// local variables
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
		
	// open file through our file system
	int	file = 0;	
	
	nrResult ret = vfs->openFile(&file, name.c_str(), true);
	if(ret != NR_OK) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_jpeg(): error open '%s' file",name.c_str());
		return shared_array<byte>(NULL);
	}

	// Set jpg-error settings
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if(setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	
	// say to jpeglib where to read data from
	jpeg_create_decompress(&cinfo);
	init_mem_source(&cinfo, vfs->getFile(file));

	// start reading of the file
	jpeg_read_header(&cinfo,TRUE);
	jpeg_start_decompress(&cinfo);
	int cont = 0;
	int row_stride = cinfo.output_width * cinfo.output_components;
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,row_stride,1);
	JSAMPLE *data_buffer = new JSAMPLE[cinfo.image_width * cinfo.image_height * cinfo.output_components];
	while(cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo,buffer,1);
		memcpy(data_buffer + cinfo.image_width * cinfo.output_components * cont,buffer[0],row_stride);
		cont++;
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	width = cinfo.image_width;
	height = cinfo.image_height;
	shared_array<byte> data (new byte[width * height * 4]);
	switch(cinfo.output_components) {
		case 1:
			for(int i = 0, j = 0; i < width * height; i++, j += 4) {
				data[j] = data[j + 1] = data[j + 2] = data_buffer[i];
				data[j + 3] = 255;
			}
			break;
		case 3:
			for(int i = 0, j = 0; i < width * height * 3; i += 3, j += 4) {
				data[j] = data_buffer[i];
				data[j + 1] = data_buffer[i + 1];
				data[j + 2] = data_buffer[i + 2];
				data[j + 3] = 255;
			}
			break;
		default:
			return shared_array<byte>(NULL);
	}
	
	vfs->closeFile(file);
	return data;
}

/**
 * save jpeg images
 * @param name - file name to which data should be saved
 * @param data - smart array containing data to be stored (must be 32Bit per Pixel)
 * @param width,height - size of the image data
 * @param quality - quality of the jpeg image (default is 80)
 * @return 0 if no error occurs
 */
int nrCTextureLoader::save_jpeg(const string& name,const shared_array<byte>& data,int width,int height,int quality) {
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	char nname[1024];
	sprintf(nname, "%s%s", nrVFS.getPathToFileSystem(), name.c_str());
	FILE *file = fopen(nname,"wb");
	if(!file) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_jpeg(): error create '%s' file",name.c_str());
		return 0;
	}
	shared_array<byte> data_buffer(new byte[width * height * 3]);
	for(int i = 0, j = 0; i < width * height * 4; i += 4, j += 3) {
		data_buffer[j + 0] = data[i + 0];
		data_buffer[j + 1] = data[i + 1];
		data_buffer[j + 2] = data[i + 2];
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo,file);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo,quality,TRUE);
	jpeg_start_compress(&cinfo,TRUE);
	int row_stride = width * 3;
	while(cinfo.next_scanline < cinfo.image_height) {
		JSAMPROW row_pointer;
		row_pointer = &data_buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo,&row_pointer,1);
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	//delete [] data_buffer;
	data_buffer.reset();
	fclose(file);
	return 1;
}

/*****************************************************************************/
/*                                                                           */
/* png images                                                                */
/*                                                                           */
/*****************************************************************************/

/* ugly, but works (unless there is corrupted data :-D) */
const byte *curr_png_ptr = NULL;
void read_mem(png_structp png_ptr, png_bytep data, unsigned int length)
{
	memcpy(data, curr_png_ptr, length);
	curr_png_ptr += length;
}   


/**
 * load png images
 * @param vfs - virtual filesystem from which file will be loaded
 * @param name - file name of the file
 * @param width,height - load_png() will return here the size of loaded image data
 * @return smart array containing data (always 32-Bit per Pixel)
 */
shared_array<byte> nrCTextureLoader::load_png(nrCFileSystem* vfs,const string& name,int &width,int &height) {

	// open file through our file system
	int	file = 0;	
	
	nrResult ret = vfs->openFile(&file, name.c_str(), true);
	if(ret != NR_OK) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_png(): error open '%s' file",name.c_str());
		return shared_array<byte>(NULL);
	}

	// read signature
	png_byte sig[8];
	vfs->readData(file,8,sig);
	if(!png_check_sig(sig,8)) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_png(): wrong signature in '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if(!png_ptr) {
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		png_destroy_read_struct(&png_ptr,0,0);
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}

	// Set up the memory reader. 
	curr_png_ptr = vfs->getFile(file)->getData();
	png_set_read_fn(png_ptr, NULL, read_mem);

	png_set_sig_bytes(png_ptr,8);
	png_read_info(png_ptr,info_ptr);
	unsigned long w,h;
	int bit_depth,color_type;
	png_get_IHDR(png_ptr,info_ptr,&w,&h,&bit_depth,&color_type,0,0,0);
	if(bit_depth == 16) png_set_strip_16(png_ptr);
	if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_expand(png_ptr);
	if(bit_depth < 8) png_set_expand(png_ptr);
	if(png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS)) png_set_expand(png_ptr);
	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
	double gamma;
	if(png_get_gAMA(png_ptr,info_ptr,&gamma)) png_set_gamma(png_ptr,(double)2.2,gamma);
	png_read_update_info(png_ptr,info_ptr);
	png_get_IHDR(png_ptr,info_ptr,&w,&h,&bit_depth,&color_type,0,0,0);
	png_uint_32 row_bytes = png_get_rowbytes(png_ptr,info_ptr);
	png_uint_32 channels = png_get_channels(png_ptr,info_ptr);
	shared_array<png_byte> img (new png_byte[row_bytes * h]);
	shared_array<png_byte*> row (new png_byte*[h]);
	for(int i = 0; i < (int)h; i++) {
		row[i] = img.get() + row_bytes * i;
	}
	png_read_image(png_ptr,row.get());
	png_read_end(png_ptr,NULL);
	png_destroy_read_struct(&png_ptr,0,0);
	vfs->closeFile(file);
	row.reset();
	width = w;
	height = h;
	shared_array<byte> data (new byte[width * height * 4]);
	byte *ptr = data.get();
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			int k = row_bytes * i + j * channels;
			*ptr++ = img[k + 0];
			*ptr++ = img[k + 1];
			*ptr++ = img[k + 2];
			if(channels == 4) *ptr++ = img[k + 3];
			else *ptr++ = 255;
		}
	}
	img.reset();
	return data;
}



/*****************************************************************************/
/*                                                                           */
/* dds images                                                                */
/*                                                                           */
/*****************************************************************************/
struct dds_colorkey {
	unsigned int dwColorSpaceLowValue;
	unsigned int dwColorSpaceHighValue;
};

struct dds_header {
	unsigned int magic;
	unsigned int dwSize;
	unsigned int dwFlags;
	unsigned int dwHeight;
	unsigned int dwWidth;
	long lPitch;
	unsigned int dwDepth;
	unsigned int dwMipMapCount;
	unsigned int dwAlphaBitDepth;
	unsigned int dwReserved;
	void *lpSurface;
	dds_colorkey ddckCKDestOverlay;
	dds_colorkey ddckCKDestBlt;
	dds_colorkey ddckCKSrcOverlay;
	dds_colorkey ddckCKSrcBlt;
	unsigned int dwPFSize;
	unsigned int dwPFFlags;
	unsigned int dwFourCC;
	unsigned int dwRGBBitCount;
	unsigned int dwRBitMask;
	unsigned int dwGBitMask;
	unsigned int dwBBitMask;
	unsigned int dwRGBAlphaBitMask;
	unsigned int dwCaps;
	unsigned int dwCaps2;
	unsigned int dwCaps3;
	unsigned int dwVolumeDepth;
	unsigned int dwcTextureStage;
};

struct dds_color {
	byte r;
	byte g;
	byte b;
};

enum {
	DDS_ERROR = -1,
	DDS_RGB,
	DDS_RGBA,
	DDS_DXT1,
	DDS_DXT2,
	DDS_DXT3,
	DDS_DXT4,
	DDS_DXT5
};

enum {
	DDPF_ALPHAPIXELS = 0x01,
	DDPF_FOURCC = 0x04,
	DDPF_RGB = 0x40,
	DDPF_RGBA = 0x41
};


/**
 * load dds images
 * @param vfs - virtual filesystem from which file will be loaded
 * @param name - file name of the file
 * @param width,height - load_jpeg() will return here the size of loaded image data
 * @return smart array containing data (always 32-Bit per Pixel)
 */
shared_array<byte> nrCTextureLoader::load_dds(nrCFileSystem* vfs,const string& name,int &width,int &height) {

	// open file through our file system
	int	file = 0;	
	
	nrResult ret = vfs->openFile(&file, name.c_str(), true);
	if(ret != NR_OK) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_dds(): error open '%s' file",name.c_str());
		return shared_array<byte>(NULL);
	}

	// read header
	dds_header header;
	vfs->readData(file, sizeof(dds_header), &header);
	if(header.magic != ('D' | 'D' << 8 | 'S' << 16 | ' ' << 24)) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_dds(): wrong magic in '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	width = header.dwWidth;
	height = header.dwHeight;
	int format = DDS_ERROR;
	if(header.dwPFFlags & DDPF_FOURCC) {
		if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('1' << 24))) format = DDS_DXT1;
		else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('2' << 24))) format = DDS_DXT2;
		else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('3' << 24))) format = DDS_DXT3;
		else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('4' << 24))) format = DDS_DXT4;
		else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('5' << 24))) format = DDS_DXT5;
	}
	else if(header.dwPFFlags == DDPF_RGB && header.dwRGBBitCount == 24) format = DDS_RGB;
	else if(header.dwPFFlags == DDPF_RGBA && header.dwRGBBitCount == 32) format = DDS_RGBA;
	if(format == DDS_ERROR) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_dds(): unknown format of '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	if(format == DDS_DXT2 || format == DDS_DXT4) {
		nrLog.Log(NR_LOG_ENGINE, 
						"nrCTextureLoader::load_dds(): DXT2 or DXT4 is not supported in '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	shared_array<byte> data (new byte[width * height * 4]);
	if(format == DDS_RGB) {
		shared_array<byte> buf (new byte[width * height * 3]);
		vfs->readData(file, width*height*3, buf.get());
		byte *src = buf.get();
		byte *dest = data.get();
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = 255;
			}
		}
		//delete [] buf;
		buf.reset();
	} else if(format == DDS_RGBA) {
		shared_array<byte> buf (new byte[width * height * 4]);
		vfs->readData (file, width*height*4, buf.get());
		byte *src = buf.get();
		byte *dest = data.get();
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
			}
		}
		//delete [] buf;
		buf.reset();
	} else {
		shared_array<byte> buf ( new byte[width * height]);
		byte *src = buf.get();
		vfs->readData (file, width*height, buf.get());
		//fread(buf,width * height,1,file);
		for(int y = 0; y < height; y += 4) {
			for(int x = 0; x < width; x += 4) {
				unsigned long long alpha = 0;
				unsigned int a0 = 0;
				unsigned int a1 = 0;
				if(format == DDS_DXT3) {
					alpha = *(unsigned long long*)src;
					src += 8;
				} else if(format == DDS_DXT5) {
					alpha = (*(unsigned long long*)src) >> 16;
					a0 = src[0];
					a1 = src[1];
					src += 8;
				}
				unsigned int c0 = *(unsigned short*)(src + 0);
				unsigned int c1 = *(unsigned short*)(src + 2);
				src += 4;
				dds_color color[4];
				color[0].r = ((c0 >> 11) & 0x1f) << 3;
				color[0].g = ((c0 >> 5) & 0x3f) << 2;
				color[0].b = (c0 & 0x1f) << 3;
				color[1].r = ((c1 >> 11) & 0x1f) << 3;
				color[1].g = ((c1 >> 5) & 0x3f) << 2;
				color[1].b = (c1 & 0x1f) << 3;
				if(c0 > c1) {
					color[2].r = (color[0].r * 2 + color[1].r) / 3;
					color[2].g = (color[0].g * 2 + color[1].g) / 3;
					color[2].b = (color[0].b * 2 + color[1].b) / 3;
					color[3].r = (color[0].r + color[1].r * 2) / 3;
					color[3].g = (color[0].g + color[1].g * 2) / 3;
					color[3].b = (color[0].b + color[1].b * 2) / 3;
				} else {
					color[2].r = (color[0].r + color[1].r) / 2;
					color[2].g = (color[0].g + color[1].g) / 2;
					color[2].b = (color[0].b + color[1].b) / 2;
					color[3].r = 0;
					color[3].g = 0;
					color[3].b = 0;
				}
				for(int i = 0; i < 4; i++) {
					unsigned int index = *src++;
					byte *dest = data.get() + (width * (y + i) + x) * 4;
					for(int j = 0; j < 4; j++) {
						*dest++ = color[index & 0x03].r;
						*dest++ = color[index & 0x03].g;
						*dest++ = color[index & 0x03].b;
						if(format == DDS_DXT1) {
							*dest++ = ((index & 0x03) == 3 && c0 <= c1) ? 0 : 255;
						} else if(format == DDS_DXT3) {
							*dest++ = (byte)((alpha & 0x0f) << 4);
							alpha >>= 4;
						} else if(format == DDS_DXT5) {
							unsigned int a = (unsigned int)(alpha & 0x07);
							if(a == 0) *dest++ = a0;
							else if(a == 1) *dest++ = a1;
							else if(a0 > a1) *dest++ = ((8 - a) * a0 + (a - 1) * a1) / 7;
							else if(a > 5) *dest++ = (a == 6) ? 0 : 255;
							else *dest++ = ((6 - a) * a0 + (a - 1) * a1) / 5;
							alpha >>= 3;
						} else *dest++ = 255;
						index >>= 2;
					}
				}
			}
		}
		//delete [] buf;
		buf.reset();
	}
	vfs->closeFile(file);
	return data;
}

/*****************************************************************************/
/*                                                                           */
/* converters                                                                */
/*                                                                           */
/*****************************************************************************/

/**
 * flip given image vertically.
 * @param data - image data 
 * @param width,height - size of the image
 * @param format - data format. Can be GL_LUMINANCE(8Bit/Pixel), GL_RGB(24Bit/Pixel), GL_RGBA(32Bit/Pixel)
 */
void nrCTextureLoader::flip (shared_array<byte>& data, int width, int height, GLuint format){
	int step = 0;
	switch (format){
		case GL_LUMINANCE:
				step = 1; break;
		case GL_RGB:
				step = 3;break;
		case GL_RGBA:
				step = 4;break;
		default:
			return;
	}
	for( int i = 0 ; i < (height / 2) ; ++i )
        for( int j = 0 ; j < width * step; j += step )
            for(int k = 0; k < step; ++k)
                swap( data[ (i * width * step) + j + k], data[ ( (height - i - 1) * width * step ) + j + k]);
}



/**
 * Convert given RGBA-Data into RGB-Data. From 32Bit/Pixel to 24Bit/Pixel
 * @param data - image data to be converted
 * @param width,height - size of the image data
 * @return new data containing the same image but in RGB-Mode
 */
shared_array<byte> nrCTextureLoader::rgba2rgb(shared_array<byte>& data,int width,int height) {
	
	shared_array<byte> dest (new byte[width * height * 3]);
	byte *d = dest.get();
	byte *s = data.get();
	for(int i = 0; i < width * height; i++) {
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		s++;
	}
	return dest;
}



/**
 * Convert given RGBA-Data into Luminance-Data. From 32Bit/Pixel to 8Bit/Pixel
 * @param data - image data to be converted
 * @param width,height - size of the image data
 * @return new data containing the same image but in Luminance(Greyscale)-Mode
 */
shared_array<byte> nrCTextureLoader::rgba2luminance(shared_array<byte>& data,int width,int height) {
	shared_array<byte> dest (new byte[width * height]);
	byte *d = dest.get();
	byte *s = data.get();
	for(int i = 0; i < width * height; i++) {
		// lese RGBA aus
		byte r = *++s;
		byte g = *++s;
		byte b = *++s;
		++s;
		
		// berechne Grauwert
		*d++ = static_cast<byte>(0.3f*float(r) + 0.59f*float(g) + 0.11f*float(b));
	}
	return dest;
}


/**
 * Convert given RGBA-Data into RGB-Data. From 32Bit/Pixel to 16Bit/Pixel
 * @param data - image data to be converted
 * @param width,height - size of the image data
 * @return new data containing the same image but in Luminance-Mode plus Alpha Chanel
 */
shared_array<byte> nrCTextureLoader::rgba2luminance_alpha(shared_array<byte>& data,int width,int height) {
	shared_array<byte> dest ( new byte[width * height * 2]);
	byte *d = dest.get();
	byte *s = data.get();
	for(int i = 0; i < width * height; i++) {
		*d++ = *s++;
		s += 2;
		*d++ = *s++;
	}
	return dest;
}

/*****************************************************************************/
/*                                                                           */
/* 3d textures                                                               */
/*                                                                           */
/*****************************************************************************/
/**
 * load 3D images
 * @param vfs - virtual filesystem from which file will be loaded
 * @param name - file name of the file
 * @param width,height,depth - load_3d() will return here the size of loaded image data
 * @param format - return here format of the 3d-data (NR_LUMINANCE, NR_LUMINANCE_ALPHA, NR_RGB, NR_RGBA)
 * @return smart array containing the data. Size of the data per Voxel is according to the format 
 *			(8,16,24,32 Bps) 
 */
shared_array<byte> nrCTextureLoader::load_3d(nrCFileSystem* vfs,const string& name,int &width,int &height,int &depth,int &format) {

	// open file through our file system
	int	file = 0;	
	
	nrResult ret = vfs->openFile(&file, name.c_str(), true);
	if(ret != NR_OK) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_3d(): error open '%s' file",name.c_str());
		return shared_array<byte>(NULL);
	}

	int magic;
	vfs->readData(file, sizeof(int), &magic);
	//fread(&magic,sizeof(int),1,file);
	if(magic != ('3' | 'D' << 8 | 'T' << 16 | 'X' << 24)) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_3d(): wrong magic in '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	
	// read data
	vfs->readData(file, sizeof(int), &width);
	vfs->readData(file, sizeof(int), &height);
	vfs->readData(file, sizeof(int), &depth);
	vfs->readData(file, sizeof(int), &format);
	
	//fread(&width,sizeof(int),1,file);
	//fread(&height,sizeof(int),1,file);
	//fread(&depth,sizeof(int),1,file);
	//fread(&format,sizeof(int),1,file);
	int size = width * height * depth;
	if(format == NR_LUMINANCE) size *= 1;
	else if(format == NR_LUMINANCE_ALPHA) size *= 2;
	else if(format == NR_RGB) size *= 3;
	else if(format == NR_RGBA) size *= 4;
	else {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::load_3d(): unknown format of '%s' file",name.c_str());
		vfs->closeFile(file);
		return shared_array<byte>(NULL);
	}
	shared_array<byte> data ( new byte[size]);
	//fread(data,sizeof(byte),size,file);
	vfs->readData(file,sizeof(byte)*size, data.get());
	vfs->closeFile(file);
	return data;
}

/**
 * save 3D images
 * @param name - file name of the file
 * @param data - smart array containing the data 
 * @param width,height,depth - size of image data
 * @param format - format of the 3d-data (LUMINANCE, LUMINANCE_ALPHA, RGB, RGBA)
 */
int nrCTextureLoader::save_3d(const string& name,const shared_array<byte>& data,int width,int height,int depth,int format) {
	char nname[1024];
	sprintf(nname, "%s%s", nrVFS.getPathToFileSystem(), name.c_str());
	FILE *file = fopen(nname,"wb");
	if(!file) {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::save_3d(): error create '%s' file",name.c_str());
		return 0;
	}
	int magic = ('3' | 'D' << 8 | 'T' << 16 | 'X' << 24);
	fwrite(&magic,sizeof(int),1,file);
	fwrite(&width,sizeof(int),1,file);
	fwrite(&height,sizeof(int),1,file);
	fwrite(&depth,sizeof(int),1,file);
	fwrite(&format,sizeof(int),1,file);
	int size = width * height * depth;
	if(format == NR_LUMINANCE) size *= 1;
	else if(format == NR_LUMINANCE_ALPHA) size *= 2;
	else if(format == NR_RGB) size *= 3;
	else if(format == NR_RGBA) size *= 4;
	else {
		nrLog.Log(NR_LOG_ENGINE, "nrCTextureLoader::save_3d(): unknown format");
	}
	fwrite(data.get(),sizeof(byte),size,file);
	fclose(file);
	return 1;
}
