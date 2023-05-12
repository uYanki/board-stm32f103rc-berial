//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#include "egl_mesh.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------


//============================================================================
// p3g_mesh
//============================================================================
p3g_mesh::p3g_mesh()
{
  m_header=0;
  m_meshlets=0;
  m_meshlet_size=0;
}
//----

p3g_mesh::p3g_mesh(const void *p3g_file_)
{
  // check for proper mesh file
  m_header=0;
  m_meshlets=0;
  m_meshlet_size=0;
  init(p3g_file_);
}
//----

void p3g_mesh::init(const void *p3g_file_)
{
  // check for proper P3G file data
  EGL_ASSERT(p3g_file_);
  EGL_ASSERT_MSG((((size_t)p3g_file_)&0x3)==0, "P3G data is not properly aligned to 4-byte boundary.\r\n");
  EGL_ASSERT_MSG(*(uint32_t*)p3g_file_==0x67643370, "Invalid P3G file signature.\r\n");
  EGL_ASSERT_MSG(((const p3g_header*)p3g_file_)->file_ver==p3g_file_ver, "P3G file %s mismatches P3G code %s.\r\n", bcd16_version_str(((const p3g_header*)p3g_file_)->file_ver).str, bcd16_version_str(p3g_file_ver).str);

  // setup members
  m_header=(const p3g_header*)p3g_file_;
  m_meshlets=(const uint8_t*)(m_header->segs+m_header->num_segs);
  uint16_t flags=m_header->flags;
  m_meshlet_size=sizeof(p3g_meshlet)-(flags&p3gflag_bvols?0:4)-(flags&p3gflag_vcones?0:4);
  EGL_ASSERT_MSG(!use_meshlet_vcones() || use_meshlet_bvols(), "Bounding volumes must be included with visibiliy cones."); // ensure bvols are present if vcones are present
}
//----------------------------------------------------------------------------
