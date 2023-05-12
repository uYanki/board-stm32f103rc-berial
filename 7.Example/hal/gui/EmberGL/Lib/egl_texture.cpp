//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#include "egl_texture.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------


//============================================================================
// texture
//============================================================================
texture::texture()
{
  m_header=0;
}
//----

texture::texture(const void *ptx_file_)
{
  m_header=0;
  init(ptx_file_);
}
//----

void texture::init(const void *ptx_file_)
{
  // check for proper P3G file data
  EGL_ASSERT(ptx_file_);
  EGL_ASSERT_MSG((((size_t)ptx_file_)&0x3)==0, "PTX data is not properly aligned to 4-byte boundary.\r\n");
  EGL_ASSERT_MSG(*(uint32_t*)ptx_file_==0x78657470, "Invalid PTX file signature.\r\n");
  EGL_ASSERT_MSG(((const ptx_header*)ptx_file_)->file_ver==ptx_file_ver, "PTX file %s mismatches PTX code %s.\r\n", bcd16_version_str(((const ptx_header*)ptx_file_)->file_ver).str, bcd16_version_str(ptx_file_ver).str);

  // setup members
  m_header=(const ptx_header*)ptx_file_;
}
//----------------------------------------------------------------------------
