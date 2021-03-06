/****************************************************************************
 *                Copyright (c) 2014 Broadcom Corporation                   *
 *                                                                          *
 *      This material is the confidential trade secret and proprietary      *
 *      information of Broadcom Corporation. It may not be reproduced,      *
 *      used, sold or transferred to any third party without the prior      *
 *      written consent of Broadcom Corporation. All rights reserved.       *
 *                                                                          *
 ****************************************************************************/

/* NOTE: this file gets exported into the Raaga Magnum host library and so it
 *       must abide by a specific strict set of rules. Please use only ANSI C
 *       constructs and include only FPSDK headers which do as well. After any
 *       edit, always make sure that the Raaga Magnum build succeeds without
 *       any warning.
 */

#include "libdspcontrol/CHIP.h"

#if !FEATURE_IS(SW_HOST, RAAGA_MAGNUM)
#  include <inttypes.h>
#  include <limits.h>
#  include <stdbool.h>
#  include <string.h>
#else
#  include "bstd_defs.h"
#endif

#include "libdspcontrol/COMMON.h"
#include "libdspcontrol/DSP.h"
#include "libdspcontrol/DSPLOG.h"
#include "libdspcontrol/TB.h"

#include "libsyschip/tbuf_chips.h"

#include "libfp/src/c_utils_internal.h"

#include "ENDIANESS.h"
#include "TB_internal.h"

/* for DDR_START */
#if IS_HOST(BM)
#  include "DSP_bm.h"
#else
#  define DDR_START     0
#endif



#if !HAS_FEATURE(TB_VARIANT) || FEATURE_IS(TB_VARIANT, NONE)
#  error "This source file is only for not 'NONE' TB variants"
#endif


size_t TB_read(TB_data_descriptor *descriptor, void *dest, size_t size, bool move_cursor)
{
    size_t available_data, remaining_size;
    uint8_t *dest_cursor;
    unsigned cur_buffer;
#if !IS_HOST(DSP_LESS)
    uint32_t cur_read_pointer;
#else
    uint8_t *cur_read_pointer;
#endif

    /* cap the read amount to the available data */
    available_data = descriptor->total_size - descriptor->cur_offset;
    if(size > available_data)
        size = available_data;

    dest_cursor = dest;
    remaining_size = size;
    cur_buffer = descriptor->cur_buffer;
    cur_read_pointer = descriptor->cur_pointer;

    while(remaining_size > 0)
    {
        /* how much can we read from this buffer? */
        size_t this_buff_size = descriptor->buffers[cur_buffer].size -
                                (cur_read_pointer - descriptor->buffers[cur_buffer].data);
        size_t this_read_size = remaining_size;
        if(this_read_size > this_buff_size)
            this_read_size = this_buff_size;

        /* read the data */
#if !IS_HOST(DSP_LESS)
        DSP_readData(descriptor->dsp, dest_cursor,
                     (uint32_t) (DDR_START + cur_read_pointer),
                     descriptor->buffers_address_space, this_read_size);
#else
        memcpy(dest_cursor, cur_read_pointer, this_read_size);
#endif

        /* update pointers */
        cur_read_pointer += this_read_size;
        dest_cursor += this_read_size;
        remaining_size -= this_read_size;
        /* if we read all from this buffer and there's more to read
           (to avoid moving beyond the last buffer), move to the next one */
        if(this_read_size == this_buff_size && remaining_size > 0)
        {
            cur_buffer++;
            cur_read_pointer = descriptor->buffers[cur_buffer].data;
        }
    }

    if(move_cursor)
        TB_seek(descriptor, size, TB_SEEK_CUR);

    return size;
}


/*
 * Always move forward. When asked to move backward, start from the beginning.
 */
size_t TB_seek(TB_data_descriptor *descriptor, int offset, TB_seek_whence whence)
{
    unsigned i;
    bool fall_through = false;

    switch(whence)
    {
    case TB_SEEK_BEGIN: ;
        if(offset <= 0)
            TB_SEEK_TO_BEGIN(descriptor)
        else if((size_t) offset >= descriptor->total_size)
            TB_SEEK_TO_END(descriptor)
        else if((size_t) offset < descriptor->cur_offset)
        {
            /* save the final value */
            descriptor->cur_offset = (size_t) offset;
            /* walk buffers */
            for(i = 0; i < descriptor->buffers_count; i++)
            {
                if((size_t) offset < descriptor->buffers[i].size)
                {
                    /* the read cursor will be inside this buffer */
                    descriptor->cur_buffer = i;
                    descriptor->cur_pointer = descriptor->buffers[i].data + ((uint32_t) offset);
                    break;
                }
                else
                {
                    /* skip this buffer */
                    offset -= descriptor->buffers[i].size;
                }
            }
        }
        else
        {
            /* the the scenario can be better handled by the TB_SEEK_CUR case */
            offset -= descriptor->cur_offset;
            fall_through = true;
        }

        if(!fall_through)
            break;
        /* no break, no KitKat (so Eclipse's happy) */

    case TB_SEEK_CUR:
        if(offset == 0)
            break;
        else if(offset < 0)
            TB_seek(descriptor, (int) descriptor->cur_offset + offset, TB_SEEK_BEGIN);
        else if((size_t) ((int) descriptor->cur_offset + offset) >= descriptor->total_size)
            TB_SEEK_TO_END(descriptor)
        else
        {
            /* save the final value */
            descriptor->cur_offset += (size_t) offset;
            /* let's pretend we are starting from the beginning of the buffer, adjust the requested offset */
            offset += descriptor->cur_pointer - descriptor->buffers[descriptor->cur_buffer].data;
            for(i = descriptor->cur_buffer; i < descriptor->buffers_count; i++)
            {
                if((size_t) offset < descriptor->buffers[i].size)
                {
                    /* the read cursor will be inside this buffer */
                    descriptor->cur_buffer = i;
                    descriptor->cur_pointer = descriptor->buffers[i].data + ((uint32_t) offset);
                    break;
                }
                else
                {
                    /* skip this buffer */
                    offset -= descriptor->buffers[i].size;
                }
            }
        }
        break;

    case TB_SEEK_END:
        if(offset >= 0)
            TB_SEEK_TO_END(descriptor)
        else if(offset <= - ((int) descriptor->total_size))
            TB_SEEK_TO_BEGIN(descriptor)
        else
            TB_seek(descriptor, (int) descriptor->total_size + offset, TB_SEEK_BEGIN);
        break;
    }

    return descriptor->cur_offset;
}


#if TARGET_BUFFER_VERSION == 2

/**
 * Reads a TB_header from descriptor (moving the cursor forward) and takes
 * care of required endianness swaps.
 *
 * @param[in]  descriptor   descriptor where to read the header from
 * @param[out] frame_header where to store the header data
 */
static inline __alwaysinline
void TB_readFrameHeader(TB_data_descriptor *descriptor, TB_header *frame_header)
{
    /* read data, it will be in little endian (FP) format */
    TB_read(descriptor, frame_header, sizeof(TB_header), true);

    /* assumptions that drive our endianness swap */
    COMPILE_TIME_ASSERT(sizeof(frame_header->prologue)            == 1);
    COMPILE_TIME_ASSERT(sizeof(frame_header->pre_padding_length)  == 1);
    COMPILE_TIME_ASSERT(sizeof(frame_header->payload_length)      == 4);
    COMPILE_TIME_ASSERT(sizeof(frame_header->post_padding_length) == 1);

    /* only payload_length needs swapping */
    frame_header->payload_length = ENDIANESS_fptoh32(frame_header->payload_length);
}


void TB_readFrameInfo(TB_data_descriptor *descriptor,
                      TB_header *frame_header,
                      TB_frame_info *ret_value,
                      TB_data_descriptor *descriptor_clone_at_payload,
                      bool move_cursor)
{
    const size_t frame_size =
            sizeof(TB_header)                 +
            frame_header->pre_padding_length  +
            frame_header->payload_length      +
            frame_header->post_padding_length;
    size_t frame_relative_off = sizeof(TB_header);  /* this is the base assumption */
    size_t saved_descriptor_offset = descriptor->cur_offset;

    /* extract info from the header */
    uint8_t frame_flags = TB_PROLOGUE_GET_FLAGS(frame_header->prologue);
    ret_value->tb_id = TB_unzip_id(TB_PROLOGUE_GET_ID(frame_header->prologue));
    ret_value->discardable = frame_flags & TB_HEADER_FLAG_DISCARDABLE;
    ret_value->payload_offset = descriptor->cur_offset + frame_header->pre_padding_length;
    ret_value->payload_address = NULL;
    ret_value->payload_length = frame_header->payload_length;

    /* do we need to clone the descriptor at payload position? */
    if(descriptor_clone_at_payload != NULL)
    {
        if(frame_header->pre_padding_length != 0)
        {
            TB_seek(descriptor, frame_header->pre_padding_length, TB_SEEK_CUR);
            frame_relative_off += frame_header->pre_padding_length;
        }
        *descriptor_clone_at_payload = *descriptor;
    }

    /* try to extract the trailer */
    if(frame_header->post_padding_length > 0)
    {
#ifdef TB_FRAMES_HAVE_TRAILER
        if(frame_flags & TB_HEADER_FLAG_HAS_TRAILER)
        {
            /* move to the trailer location */
            size_t trailer_position_offset = frame_size -
                                             sizeof(TB_trailer) -
                                             frame_relative_off;
            if(trailer_position_offset > 0)
                TB_seek(descriptor,
                        trailer_position_offset,
                        TB_SEEK_CUR);

            /* read trailer */
            TB_trailer frame_trailer;
            COMPILE_TIME_ASSERT(sizeof(TB_trailer) == 1);   /* otherwise we should deal with endianness */
            TB_read(descriptor, &frame_trailer, sizeof(TB_trailer), true);
            ret_value->has_trailer_info = true;
            ret_value->trailer_info.overwritten = frame_trailer & TB_TRAILER_OVERWRITTEN_BIT;
            ret_value->trailer_info.chunks_count = frame_trailer & TB_TRAILER_CHUNKS_COUNT_MASK;

            frame_relative_off += trailer_position_offset + sizeof(TB_trailer);
        }
        else
#endif
        {
            /* no trailer */
            ret_value->has_trailer_info = false;
        }
    }
    else
        ret_value->has_trailer_info = false;

    /* fulfil the move_cursor request */
    if(!move_cursor && frame_relative_off != sizeof(TB_header))
        TB_seek(descriptor, saved_descriptor_offset, TB_SEEK_BEGIN);
    if(move_cursor)
    {
        size_t final_position_offset = frame_size - frame_relative_off;
        if(final_position_offset > 0)
            TB_seek(descriptor,
                    final_position_offset,
                    TB_SEEK_CUR);
    }
}


bool TB_services_filter(TB_frame_info *frame_info, void *data)
{
    TB_services_filter_config *config = (TB_services_filter_config *) data;
    if(frame_info->discardable && !config->accept_overwritten_frames)
        return false;

    if(TB_SERVICE_FLAG_FROM_ID(frame_info->tb_id.service_id) & config->accepted_services)
        return true;
    else
        return false;
}


size_t TB_countFrames(TB_data_descriptor *descriptor,
                      unsigned max_frames,
                      unsigned *frames_count,
                      size_t *available_payload,
                      size_t *max_payload_size,
                      TB_frame_filter filter,
                      void *filter_data)
{
    /* reset values */
    *frames_count = 0;
    *available_payload = 0;
    *max_payload_size = 0;
    /* nothing to do? */
    if(max_frames == 0)
        return descriptor->cur_offset;

    /* save current buffer position */
    size_t saved_descriptor_offset = descriptor->cur_offset;

    /* walk the frames */
    unsigned total_frames = 0;
    size_t total_payload_size = 0;
    size_t biggest_payload_size = 0;
    size_t remaining_data = descriptor->total_size - descriptor->cur_offset;
    size_t cursor_after_last_frame = descriptor->cur_offset;
    while(remaining_data >= sizeof(TB_header) &&
          total_frames < max_frames)
    {
        /* read the header */
        TB_header frame_header;
        TB_readFrameHeader(descriptor, &frame_header);
        DSPLOG_JUNK("TB_countFrames read header prologue=%#x, post_padding_length=%"PRIu8","
                    " payload_length=%"PRIu32", pre_padding_length=%"PRIu8,
                    frame_header.prologue, frame_header.pre_padding_length,
                    frame_header.payload_length, frame_header.post_padding_length);

        /* is this frame complete? */
        size_t frame_size = sizeof(TB_header)               +
                            frame_header.payload_length     +
                            frame_header.pre_padding_length +
                            frame_header.post_padding_length;
        if(remaining_data < frame_size)
            break;      /* incomplete frame, exit here */

        if(filter != NULL)
        {
            /* we should extract frame info to allow the filter to take a decision */
            TB_frame_info frame_info;
            TB_readFrameInfo(descriptor, &frame_header, &frame_info, NULL, true);

            if(filter(&frame_info, filter_data))
            {
                /* frame accepted */
                total_frames++;
                total_payload_size += frame_header.payload_length;
                if(frame_header.payload_length > biggest_payload_size)
                    biggest_payload_size = frame_header.payload_length;
            }
            /* frame rejected, nothing to to */
        }
        else
        {
            /* update counters */
            total_frames++;
            total_payload_size += frame_header.payload_length;
            if(frame_header.payload_length > biggest_payload_size)
                biggest_payload_size = frame_header.payload_length;

            /* skip to the next frame */
            TB_seek(descriptor,
                    frame_header.pre_padding_length +
                    frame_header.payload_length +
                    frame_header.post_padding_length,
                    TB_SEEK_CUR);
        }

        /* we have moved because of TB_readFrameInfo or TB_seek,
         * update the "byte after the last considered frame" position */
        cursor_after_last_frame = descriptor->cur_offset;

        /* update the remaining available data */
        remaining_data -= frame_size;
    }

    /* restore read cursor position */
    TB_seek(descriptor, saved_descriptor_offset, TB_SEEK_BEGIN);

    /* update out parameters */
    *frames_count = total_frames;
    *available_payload = total_payload_size;
    *max_payload_size = biggest_payload_size;

    return cursor_after_last_frame;
}


size_t TB_peekFrames(TB_data_descriptor *descriptor,
                     TB_frame_info *frame_info,
                     unsigned num_frame_info,
                     unsigned *frames_count,
                     size_t *available_payload,
                     TB_frame_filter filter,
                     void *filter_data)
{
    /* reset values */
    *frames_count = 0;
    *available_payload = 0;
    /* nothing to do? */
    if(num_frame_info == 0)
        return descriptor->cur_offset;

    /* save current buffer position */
    size_t saved_descriptor_offset = descriptor->cur_offset;

    /* walk the frames */
    unsigned total_frames = 0;
    size_t total_payload_size = 0;
    size_t remaining_data = descriptor->total_size - descriptor->cur_offset;
    size_t cursor_after_last_frame = descriptor->cur_offset;
    while(remaining_data >= sizeof(TB_header) &&
          total_frames < num_frame_info)
    {
        /* read the header */
        TB_header frame_header;
        TB_readFrameHeader(descriptor, &frame_header);

        /* is this frame complete? */
        size_t frame_size = sizeof(TB_header)               +
                            frame_header.payload_length     +
                            frame_header.pre_padding_length +
                            frame_header.post_padding_length;
        if(remaining_data < frame_size)
            break;      /* incomplete frame, exit here */

        /* extract frame info */
        TB_frame_info *cur_frame_info = &frame_info[total_frames];
        TB_readFrameInfo(descriptor, &frame_header, cur_frame_info, NULL, true);

        if(filter == NULL ||
           filter(cur_frame_info, filter_data) == true)
        {
            /* frame accepted */
            total_frames++;
            total_payload_size += frame_header.payload_length;
        }
        /* frame rejected, track only where we read up to */
        cursor_after_last_frame = descriptor->cur_offset;

        /* update the remaining available data */
        remaining_data -= frame_size;
    }

    /* restore read cursor position */
    TB_seek(descriptor, saved_descriptor_offset, TB_SEEK_BEGIN);

    /* update out parameters */
    *frames_count = total_frames;
    *available_payload = total_payload_size;

    return cursor_after_last_frame;
}


size_t TB_readAllFrames(TB_data_descriptor *descriptor,
                        TB_frame_info *frame_info,
                        unsigned num_frame_info,
                        unsigned *frames_count,
                        void *buf,
                        size_t buf_size,
                        size_t *available_payload,
                        TB_frame_filter filter,
                        void *filter_data)
{
    /* reset values */
    *frames_count = 0;
    *available_payload = 0;
    /* nothing to do? */
    if(num_frame_info == 0)
        return descriptor->cur_offset;

    /* save current buffer position */
    size_t saved_descriptor_offset = descriptor->cur_offset;

    /* walk the frames */
    unsigned total_frames = 0;
    size_t total_payload_size = 0;
    uint8_t *write_cursor = buf;
    size_t write_remaining_data = buf_size;
    size_t remaining_data = descriptor->total_size - descriptor->cur_offset;
    size_t cursor_after_last_frame = descriptor->cur_offset;
    while(remaining_data >= sizeof(TB_header) &&
          total_frames < num_frame_info)
    {
        /* read the header */
        TB_header frame_header;
        TB_readFrameHeader(descriptor, &frame_header);

        /* is this frame complete? */
        size_t frame_size = sizeof(TB_header)               +
                            frame_header.payload_length     +
                            frame_header.pre_padding_length +
                            frame_header.post_padding_length;
        if(remaining_data < frame_size)
            break;      /* incomplete frame, exit here */

        /* extract frame info */
        TB_frame_info *cur_frame_info = &frame_info[total_frames];
        TB_data_descriptor descriptor_at_payload;
        TB_readFrameInfo(descriptor,
                         &frame_header,
                         cur_frame_info,
                         &descriptor_at_payload,
                         true);

        /* check the filter response */
        bool frame_accepted = true;
        if(filter != NULL)
           frame_accepted = filter(cur_frame_info, filter_data);

        /* lack of available space is important only if the frame has been accepted */
        if(frame_accepted && write_remaining_data < frame_header.payload_length)
            break;  /* we exhausted write space, quit */

        /* how to proceed? */
        if(!frame_accepted)
        {
            /* frame rejected, update only relevant state */
            remaining_data -= frame_size;
            cursor_after_last_frame = descriptor->cur_offset;
        }
        else
        {
            /* frame accepted, read payload */
            TB_read(&descriptor_at_payload, write_cursor, frame_header.payload_length, false);
            cur_frame_info->payload_address = write_cursor;
            /* update state */
            total_frames++;
            total_payload_size += frame_header.payload_length;
            write_cursor += frame_header.payload_length;
            write_remaining_data -= frame_header.payload_length;
            cursor_after_last_frame = descriptor->cur_offset;
        }
    }

    /* restore read cursor position */
    TB_seek(descriptor, saved_descriptor_offset, TB_SEEK_BEGIN);

    /* update out parameters */
    *frames_count = total_frames;
    *available_payload = total_payload_size;

    return cursor_after_last_frame;
}


size_t TB_readSingleFrames(TB_data_descriptor *descriptor,
                           TB_frame_reader frames_reader,
                           void *reader_data)
{
     /* save current buffer position */
     size_t saved_descriptor_offset = descriptor->cur_offset;

     /* walk the frames */
     size_t remaining_data = descriptor->total_size - descriptor->cur_offset;
     size_t cursor_after_last_frame = descriptor->cur_offset;
     while(remaining_data >= sizeof(TB_header))
     {
         /* read the header */
         TB_header frame_header;
         TB_readFrameHeader(descriptor, &frame_header);

         /* is this frame complete? */
         size_t frame_size = sizeof(TB_header)               +
                             frame_header.payload_length     +
                             frame_header.pre_padding_length +
                             frame_header.post_padding_length;
         if(remaining_data < frame_size)
             break;      /* incomplete frame, exit here */

         /* extract frame info */
         TB_frame_info frame_info;
         TB_data_descriptor descriptor_at_payload;
         TB_readFrameInfo(descriptor,
                          &frame_header,
                          &frame_info,
                          &descriptor_at_payload,
                          true);

         /* invoke the callback */
         bool should_continue = frames_reader(&frame_info,
                                              &descriptor_at_payload,
                                              reader_data);
         if(!should_continue)
             break;

         /* update only if frames_reader returned true */
         cursor_after_last_frame = descriptor->cur_offset;

         /* update the remaining available data */
         remaining_data -= frame_size;
     }

     /* restore original read read cursor position */
     TB_seek(descriptor, saved_descriptor_offset, TB_SEEK_BEGIN);

     return cursor_after_last_frame;
}

#endif  /* TARGET_BUFFER_VERSION == 2 */
