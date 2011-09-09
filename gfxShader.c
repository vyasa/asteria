/****************************************************************************
 *                                                                          *
 * This file is part of the Asteria project.                                *
 * Copyright (C) 2011 Samuel C. Payson, Akanksha Vyas                       *
 *                                                                          *
 * Asteria is free software: you can redistribute it and/or modify it under *
 * the terms of the GNU General Public License as published by the Free     *
 * Software Foundation, either version 3 of the License, or (at your        *
 * option) any later version.                                               *
 *                                                                          *
 * Asteria is distributed in the hope that it will be useful, but WITHOUT   *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    *
 * for more details.                                                        *
 *                                                                          *
 * You should have received a copy of the GNU General Public License along  *
 * with Asteria. If not, see <http://www.gnu.org/licenses/>.                *
 *                                                                          *
 ****************************************************************************/

/* gfxShader.c */
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libInclude.h"

/* Support shader source files up to 256k in size. */
static char shaderBuffer[1 << 18];

/* FIXME: Need to find a way to report error messages. */
GLuint gfxLoadShader( const char * shader ) {
   GLuint ret;
   FILE * input;
   const char * ext;
   struct stat st;
   int read;

   stat( shader, &st );

   if ( st.st_size > ( 1 << 18 ) ) {
      return 0;
   } else if ( NULL == ( input = fopen( shader, "r" ) ) ) {
      return 0;
   } else if ( NULL == ( ext = strrchr( shader, '.' ) ) ) {
      fclose( input );
      return 0;
   }

   read = fread( shaderBuffer, 1, 1 << 18, input );
   shaderBuffer[read] = '\0';
   fclose( input );

   if ( 0 == strcmp( ext, ".vtx" ) ) {
      ret = glCreateShader( GL_VERTEX_SHADER );
   } else if ( 0 == strcmp( ext, ".frg" ) ) {
      ret = glCreateShader( GL_FRAGMENT_SHADER );
   } else {
      return 0;
   }

   glShaderSource( ret, 1, (const GLchar **)&shaderBuffer, NULL );
   glCompileShader( ret );

   return ret;
}

GLuint gfxMakeShaderProgram( GLuint vtx, GLuint frg ) {
   GLuint ret;

   ret = glCreateProgram();

   glAttachShader( ret, vtx );
   glAttachShader( ret, frg );

   glLinkProgram( ret );

   return ret;

}