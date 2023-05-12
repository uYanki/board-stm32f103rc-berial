//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_RASTERIZER_CONFIG_H
#define EGL_RASTERIZER_CONFIG_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_core.h"
EGL_NAMESPACE_BEGIN

// new
#define EGL_BUILDOP_RASTERIZER_HIZ 1
#define EGL_BUILDOP_DMA_TRANSFER 1
#ifdef EGL_BUILDOP_DEBUG_LOGS
#define EGL_BUILDOP_RASTERIZER_STATS 1
#else
#define EGL_BUILDOP_RASTERIZER_STATS 0
#endif
typedef uint16_t rasterizer_vertex_cache_offset_t;
typedef uint16_t rasterizer_local_cluster_index_t;   // clusters for single PSO dispatch
typedef uint16_t rasterizer_global_cluster_index_t;  // clusters for all PSO dispatches in a frame
typedef uint16_t rasterizer_cluster_strip_index_t;   // cluster strips in a frame
typedef uint16_t rasterizer_dispatch_index_t;        // dispatch calls in a frame
typedef uint8_t rasterizer_tile_size_t;              // tile size
enum {rasterizer_subpixel_bits=4};                   // rasterization sub-pixel precision
enum {rasterizer_hiz_tile_width=16};                 // hi-z tile size
enum {rasterizer_tform_cache_block_size=16};         // transform cache block size in bytes (allocation granularity)
enum {rasterizer_max_rts=4};                         // max number of render targets
//----
enum {rasterizer_max_local_clusters=(rasterizer_local_cluster_index_t(-1)>>1)+1};
enum {rasterizer_hiz_tile_pixels=rasterizer_hiz_tile_width*rasterizer_hiz_tile_width};
enum {rasterizer_subpixel_bitmask=rasterizer_subpixel_bits?(1<<rasterizer_subpixel_bits)-1:0};
enum {rasterizer_half_subpixel_offsx=rasterizer_subpixel_bits?(1<<(rasterizer_subpixel_bits-1))-1:0};
enum {rasterizer_half_subpixel_offsy=rasterizer_subpixel_bits?(1<<(rasterizer_subpixel_bits-1)):0};
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
