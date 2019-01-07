/*
 * File      : bxcan.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-06     sundm75       first version
 */

#ifndef LS1C_DRV_CAN_H
#define LS1C_DRV_CAN_H

#define USING_BXCAN0

#define LPC_CAN_AF_STD_INIT(id) \
     RT_CAN_FILTER_ITEM_INIT(id,0,0,0,0xFFFFFFFF)
#define LPC_CAN_AF_EXT_INIT(id) \
     RT_CAN_FILTER_ITEM_INIT(id,1,0,0,0xFFFFFFFF)
#define LPC_CAN_AF_STD_GRP_INIT(id1,id2) \
     RT_CAN_FILTER_ITEM_INIT(id1,0,0,1,id2)
#define LPC_CAN_AF_EXT_GRP_INIT(id1,id2) \
     RT_CAN_FILTER_ITEM_INIT(id1,1,0,1,id2)

extern int ls1c_bxcan_init(void);

#endif /*DRV_CAN_H_*/
