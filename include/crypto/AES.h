//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								AES.h
// SUBSYSTEM:						C++ Wrapper for AES encryption.
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2026 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
//
// OVERVIEW:						C++ Wrapper for AES encryption. Uses libssl.
//
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
//
// HISTORY:             20126-03-28 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef GCL_CRYPTO_AES_H
#define GCL_CRYPTO_AES_H

#include <cstdint>

//#define INCLUDE_AES_CIPHER
#define INCLUDE_AES_INVCIPHER

#define AES_BLOCKSIZE 4

enum
{
  AES_128 = 0,
  AES_192 = 1,
  AES_256 = 2,
  AES_MAX = 3,
};

#ifdef INCLUDE_AES_CIPHER
void AES_cipher(uint8_t aesFlavour, uint8_t const *aesKey, uint8_t *aesData);
#endif

#ifdef INCLUDE_AES_INVCIPHER
void AES_invCipher(uint8_t aesFlavour, uint8_t const *aesKey, uint8_t *aesData);
#endif




#endif /* PIC_LIB_INCLUDE_CRYPTO_AES_H_ */

#endif // AES_H
