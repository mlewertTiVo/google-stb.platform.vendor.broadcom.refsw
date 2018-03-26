/*
 * Generic Broadcom Home Networking Division (HND) DMA transmit routines.
 * This supports the following chips: BCM42xx, 44xx, 47xx .
 *
 * Copyright (C) 2017, Broadcom. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id$
 */

/**
 * @file
 * @brief
 * Source file for HNDDMA module. This file contains the functionality for the TX data path.
 */

#include <bcm_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <bcmdevs.h>
#include <osl.h>
#include <bcmendian.h>
#include <hndsoc.h>
#include <bcmutils.h>
#include <siutils.h>

#include <sbhnddma.h>
#include <hnddma.h>
#include "hnddma_priv.h"

#ifdef BCMLFRAG
static int dma64_txfast_lfrag(dma_info_t *di, void *p0, bool commit);
static int dma64_txfast_sfd(dma_info_t *di, void *p0, bool commit);
#endif /* BCMLFRAG */

void
dma32_txreclaim(dma_info_t *di, txd_range_t range)
{
	void *p;

	DMA_TRACE(("%s: dma_txreclaim %s\n", di->name,
	           (range == HNDDMA_RANGE_ALL) ? "all" :
	           ((range == HNDDMA_RANGE_TRANSMITTED) ? "transmitted" : "transfered")));

	if (di->txin == di->txout)
		return;

	while ((p = dma32_getnexttxp(di, range)))
		PKTFREE(di->osh, p, TRUE);
}

/**
 * Helper routine to do commit only operation on descriptors
 * Externally accessible version
 */
BCMFASTPATH void
dma32_txcommit(dma_info_t *di)
{
	dma32_txcommit_local(di);
}

/**
 * !! tx entry routine
 * supports full 32bit dma engine buffer addressing so
 * dma buffers can cross 4 Kbyte page boundaries.
 *
 * WARNING: call must check the return value for error.
 *   the error(toss frames) could be fatal and cause many subsequent hard to debug problems
 */
int
dma32_txfast(dma_info_t *di, void *p0, bool commit)
{
	void *p, *next;
	uchar *data;
	uint len;
	uint16 txout;
	uint32 flags = 0;
	dmaaddr_t pa;

	DMA_TRACE(("%s: dma_txfast\n", di->name));

	txout = di->txout;

	/*
	 * Walk the chain of packet buffers
	 * allocating and initializing transmit descriptor entries.
	 */
	for (p = p0; p; p = next) {
		uint nsegs, j;
		hnddma_seg_map_t *map;

		data = PKTDATA(di->osh, p);
		len = PKTLEN(di->osh, p);
#ifdef BCM_DMAPAD
		if (DMAPADREQUIRED(di)) {
			len += PKTDMAPAD(di->osh, p);
		}
#endif
		next = PKTNEXT(di->osh, p);

		/* return nonzero if out of tx descriptors */
		if (NEXTTXD(txout) == di->txin)
			goto outoftxd;

		if (len == 0)
			continue;

		if (DMASGLIST_ENAB)
			bzero(&di->txp_dmah[txout], sizeof(hnddma_seg_map_t));

		/* get physical address of buffer start */
#ifdef BCM_SECURE_DMA
		pa = SECURE_DMA_MAP(di->osh, data, len, DMA_TX, p, &di->txp_dmah[txout],
			&di->sec_cma_info_tx, 0);
#else
		pa = DMA_MAP(di->osh, data, len, DMA_TX, p, &di->txp_dmah[txout]);
#endif /* BCM_SECURE_DMA */
		if (DMASGLIST_ENAB) {
			map = &di->txp_dmah[txout];

			/* See if all the segments can be accounted for */
			if (map->nsegs > (uint)(di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1))
				goto outoftxd;

			nsegs = map->nsegs;
		} else
			nsegs = 1;

		for (j = 1; j <= nsegs; j++) {
			flags = 0;
			if (p == p0 && j == 1)
				flags |= CTRL_SOF;

			/* With a DMA segment list, Descriptor table is filled
			 * using the segment list instead of looping over
			 * buffers in multi-chain DMA. Therefore, EOF for SGLIST is when
			 * end of segment list is reached.
			 */
			if ((!DMASGLIST_ENAB && next == NULL) ||
			    (DMASGLIST_ENAB && j == nsegs))
				flags |= (CTRL_IOC | CTRL_EOF);
			if (txout == (di->ntxd - 1))
				flags |= CTRL_EOT;

			if (DMASGLIST_ENAB) {
				len = map->segs[j - 1].length;
				pa = map->segs[j - 1].addr;
			}
			ASSERT(PHYSADDRHI(pa) == 0);

			dma32_dd_upd(di, di->txd32, pa, txout, &flags, len);
			ASSERT(di->txp[txout] == NULL);

			txout = NEXTTXD(txout);
		}

		/* See above. No need to loop over individual buffers */
		if (DMASGLIST_ENAB)
			break;
	}

	/* if last txd eof not set, fix it */
	if (!(flags & CTRL_EOF))
		W_SM(&di->txd32[PREVTXD(txout)].ctrl, BUS_SWAP32(flags | CTRL_IOC | CTRL_EOF));

	/* save the packet */
	di->txp[PREVTXD(txout)] = p0;

	/* bump the tx descriptor index */
	di->txout = txout;

	/* kick the chip */
	if (commit) {
		dma32_txcommit_local(di);
	}

	/* tx flow control */
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (0);

outoftxd:
	DMA_ERROR(("%s: dma_txfast: out of txds\n", di->name));
	di->hnddma.txavail = 0;
	di->hnddma.txnobuf++;
	di->hnddma.txnodesc++;
	return (-1);
}

/**
 * Reclaim next completed txd (txds if using chained buffers) in the range
 * specified and return associated packet.
 * If range is HNDDMA_RANGE_TRANSMITTED, reclaim descriptors that have be
 * transmitted as noted by the hardware "CurrDescr" pointer.
 * If range is HNDDMA_RANGE_TRANSFERED, reclaim descriptors that have be
 * transfered by the DMA as noted by the hardware "ActiveDescr" pointer.
 * If range is HNDDMA_RANGE_ALL, reclaim all txd(s) posted to the ring and
 * return associated packet regardless of the value of hardware pointers.
 */
void *
dma32_getnexttxp(dma_info_t *di, txd_range_t range)
{
	uint16 start, end, i;
	uint16 active_desc;
	void *txp;

	DMA_TRACE(("%s: dma_getnexttxp %s\n", di->name,
	           (range == HNDDMA_RANGE_ALL) ? "all" :
	           ((range == HNDDMA_RANGE_TRANSMITTED) ? "transmitted" : "transfered")));

	if (di->ntxd == 0)
		return (NULL);

	txp = NULL;

	start = di->txin;
	if (range == HNDDMA_RANGE_ALL)
		end = di->txout;
	else {
		dma32regs_t *dregs = di->d32txregs;

		if (di->txin == di->xs0cd) {
		end = (uint16)B2I(R_REG(di->osh, &dregs->status) & XS_CD_MASK, dma32dd_t);
			di->xs0cd = end;
		} else
			end = di->xs0cd;

		if (range == HNDDMA_RANGE_TRANSFERED) {
			active_desc = (uint16)((R_REG(di->osh, &dregs->status) & XS_AD_MASK) >>
			                       XS_AD_SHIFT);
			active_desc = (uint16)B2I(active_desc, dma32dd_t);
			if (end != active_desc)
				end = PREVTXD(active_desc);
		}
	}

	if ((start == 0) && (end > di->txout))
		goto bogus;

	for (i = start; i != end && !txp; i = NEXTTXD(i)) {
		dmaaddr_t pa;
		hnddma_seg_map_t *map = NULL;
		uint size, j, nsegs;

		PHYSADDRLOSET(pa, (BUS_SWAP32(R_SM(&di->txd32[i].addr)) - di->dataoffsetlow));
		PHYSADDRHISET(pa, 0);

		if (DMASGLIST_ENAB) {
			map = &di->txp_dmah[i];
			size = map->origsize;
			nsegs = map->nsegs;
		} else {
			size = (BUS_SWAP32(R_SM(&di->txd32[i].ctrl)) & CTRL_BC_MASK);
			nsegs = 1;
		}

		for (j = nsegs; j > 0; j--) {
#if defined(DESCR_DEADBEEF)
			W_SM(&di->txd32[i].addr, 0xdeadbeef);
#endif

			txp = di->txp[i];
			di->txp[i] = NULL;
			if (j > 1)
				i = NEXTTXD(i);
		}
#ifdef BCM_SECURE_DMA
		SECURE_DMA_UNMAP(di->osh, pa, size, DMA_TX, NULL, NULL,
			&di->sec_cma_info_tx, 0);
#else
		DMA_UNMAP(di->osh, pa, size, DMA_TX, txp, map);
#endif /* BCM_SECURE_DMA */
	}

	di->txin = i;

	/* tx flow control */
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (txp);

bogus:
	DMA_NONE(("dma_getnexttxp: bogus curr: start %d end %d txout %d force %d\n",
	          start, end, di->txout, forceall));
	return (NULL);
}

void BCMFASTPATH
dma64_txreclaim(dma_info_t *di, txd_range_t range)
{
	void *p;

	DMA_TRACE(("%s: dma_txreclaim %s\n", di->name,
	           (range == HNDDMA_RANGE_ALL) ? "all" :
	           ((range == HNDDMA_RANGE_TRANSMITTED) ? "transmitted" : "transfered")));

	if (di->txin == di->txout)
		return;

	/* if this is decriptor only DMA then just reset the txin. No data packets to free */
	if (di->hnddma.dmactrlflags & DMA_CTRL_DESC_ONLY_FLAG) {
		DMA_TRACE(("%s: DESC only DMA. Seting txin=txout=%d \n", di->name, di->txout));
		di->txin = di->txout;
		di->hnddma.txavail = di->ntxd - 1;
		return;
	}

	/* if using indirect DMA access, then configure IndQSel */
	if (DMA_INDIRECT(di)) {
		dma_set_indqsel((hnddma_t *)di, FALSE);
	}

	while ((p = dma64_getnexttxp(di, range))) {
		/* For unframed data, we don't have any packets to free */
		if (!(di->hnddma.dmactrlflags & DMA_CTRL_UNFRAMED))
			PKTFREE(di->osh, p, TRUE);
	}
}

/**
 * !! tx entry routine
 * WARNING: call must check the return value for error.
 *   the error(toss frames) could be fatal and cause many subsequent hard to debug problems
 */
int BCMFASTPATH
dma64_txfast(dma_info_t *di, void *p0, bool commit)
{
	void *p, *next;
	uchar *data;
	uint len;
	uint16 txout;
	uint32 flags = 0;
	dmaaddr_t pa;
	bool war;

	DMA_TRACE(("%s: dma_txfast\n", di->name));

	/* if using indirect DMA access, then configure IndQSel */
	if (DMA_INDIRECT(di)) {
		dma_set_indqsel((hnddma_t *)di, FALSE);
	}

	/* new DMA routine for LFRAGS */
#ifdef BCMLFRAG
	if (BCMLFRAG_ENAB()) {
		if (PKTISSFDFRAME(di->osh, p0)) {
			return dma64_txfast_sfd(di, p0, commit);
		}

		if (PKTISTXFRAG(di->osh, p0)) {
			return dma64_txfast_lfrag(di, p0, commit);
		}
	}
#endif

	txout = di->txout;
	war = (di->hnddma.dmactrlflags & DMA_CTRL_DMA_AVOIDANCE_WAR) ? TRUE : FALSE;

	/*
	 * Walk the chain of packet buffers
	 * allocating and initializing transmit descriptor entries.
	 */
	for (p = p0; p; p = next) {
		uint nsegs, j, segsadd;
		hnddma_seg_map_t *map = NULL;

		data = PKTDATA(di->osh, p);
		len = PKTLEN(di->osh, p);
		next = PKTNEXT(di->osh, p);
#ifdef BCM_DMAPAD
		if (DMAPADREQUIRED(di)) {
			len += PKTDMAPAD(di->osh, p);
		}
#endif /* BCM_DMAPAD */

		/* return nonzero if out of tx descriptors */
		if (NEXTTXD(txout) == di->txin)
			goto outoftxd;

		if (len == 0)
			continue;

		/* get physical address of buffer start */

#if !defined(BCM_SECURE_DMA)
		if (DMASGLIST_ENAB)
			bzero(&di->txp_dmah[txout], sizeof(hnddma_seg_map_t));
#endif

#ifdef BCM_SECURE_DMA
		if (DMASGLIST_ENAB) {
			bzero(&di->txp_dmah[txout], sizeof(hnddma_seg_map_t));
			pa = SECURE_DMA_MAP(di->osh, data, len, DMA_TX, p,
				&di->txp_dmah[txout], &di->sec_cma_info_tx, 0);
		} else {
			pa = SECURE_DMA_MAP(di->osh, data, len, DMA_TX, NULL, NULL,
				&di->sec_cma_info_tx, 0);
		}
#else
		pa = DMA_MAP(di->osh, data, len, DMA_TX, p, &di->txp_dmah[txout]);
#endif /* BCM_SECURE_DMA */

		if (DMASGLIST_ENAB) {
			map = &di->txp_dmah[txout];

			/* See if all the segments can be accounted for */
			if (map->nsegs > (uint)(di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1))
				goto outoftxd;

			nsegs = map->nsegs;
		} else
			nsegs = 1;

		segsadd = 0;
		for (j = 1; j <= nsegs; j++) {
			flags = 0;

			if (p == p0 && j == 1)
				flags |= D64_CTRL1_SOF;

			/* With a DMA segment list, Descriptor table is filled
			 * using the segment list instead of looping over
			 * buffers in multi-chain DMA. Therefore, EOF for SGLIST is when
			 * end of segment list is reached.
			 */
			if ((!DMASGLIST_ENAB && next == NULL) ||
			    (DMASGLIST_ENAB && j == nsegs)) {
				/* Set "interrupt on completion" bit only on last commit packet
				 * to reduce the Tx completion event
				 */
				flags |= D64_CTRL1_EOF;
				if (commit)
					flags |= D64_CTRL1_IOC;
			}

			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;

			if (di->burstsize_ctrl || (CHIPID(di->sih->chip) == BCM7271_CHIP_ID))
				flags |= D64_CTRL1_NOTPCIE;

			if (DMASGLIST_ENAB) {
				len = map->segs[j - 1].length;
				pa = map->segs[j - 1].addr;
				if (len > 128 && war) {
					uint remain, new_len, align64;
					/* check for 64B aligned of pa */
					align64 = (uint)(PHYSADDRLO(pa) & 0x3f);
					align64 = (64 - align64) & 0x3f;
					new_len = len - align64;
					remain = new_len % 128;
					if (remain > 0 && remain <= 4) {
						uint32 buf_addr_lo;
						uint32 tmp_flags =
							flags & (~(D64_CTRL1_EOF | D64_CTRL1_IOC));
						flags &= ~(D64_CTRL1_SOF | D64_CTRL1_EOT);
						remain += 64;
						dma64_dd_upd(di, di->txd64, pa, txout,
							&tmp_flags, len-remain);
						ASSERT(di->txp[txout] == NULL);
						txout = NEXTTXD(txout);
						/* return nonzero if out of tx descriptors */
						if (txout == di->txin) {
							DMA_ERROR(("%s: dma_txfast: Out-of-DMA"
								" descriptors (txin %d txout %d"
								" nsegs %d)\n", __FUNCTION__,
								di->txin, di->txout, nsegs));
							goto outoftxd;
						}
						if (txout == (di->ntxd - 1))
							flags |= D64_CTRL1_EOT;
						buf_addr_lo = PHYSADDRLO(pa);
						PHYSADDRLOSET(pa, (PHYSADDRLO(pa) + (len-remain)));
						if (PHYSADDRLO(pa) < buf_addr_lo) {
							PHYSADDRHISET(pa, (PHYSADDRHI(pa) + 1));
						}
						len = remain;
						segsadd++;
						di->dma_avoidance_cnt++;
					}
				}
			}
			dma64_dd_upd(di, di->txd64, pa, txout, &flags, len);
			ASSERT(di->txp[txout] == NULL);

			txout = NEXTTXD(txout);
			/* return nonzero if out of tx descriptors */
			if (txout == di->txin) {
				DMA_ERROR(("%s: dma_txfast: Out-of-DMA descriptors"
					   " (txin %d txout %d nsegs %d)\n", __FUNCTION__,
					   di->txin, di->txout, nsegs));
				goto outoftxd;
			}
		}
		if (segsadd && DMASGLIST_ENAB)
			map->nsegs += segsadd;

		/* See above. No need to loop over individual buffers */
		if (DMASGLIST_ENAB)
			break;
	}

	/* if last txd eof not set, fix it */
	if (!(flags & D64_CTRL1_EOF))
		W_SM(&di->txd64[PREVTXD(txout)].ctrl1,
		     BUS_SWAP32(flags | D64_CTRL1_IOC | D64_CTRL1_EOF));

	/* save the packet */
	di->txp[PREVTXD(txout)] = p0;

#if defined(BULK_DESCR_FLUSH)
	{
		uint32 flush_cnt = NTXDACTIVE(di->txout, txout);
		if (txout < di->txout) {
			DMA_FLUSH(di->osh, dma64_txd64(di, 0), DMA64_FLUSH_LEN(txout),
			        DMA_TX, NULL, NULL);
			flush_cnt -= txout;
		}
		DMA_FLUSH(di->osh, dma64_txd64(di, di->txout), DMA64_FLUSH_LEN(flush_cnt),
		        DMA_TX, NULL, NULL);
	}
#endif  /* BULK_DESCR_FLUSH */

	/* bump the tx descriptor index */
	di->txout = txout;

	/* kick the chip */
	if (commit) {
		dma64_txcommit_local(di);
	}

	/* tx flow control */
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (0);

outoftxd:
	DMA_ERROR(("%s: dma_txfast: out of txds !!!\n", di->name));
	di->hnddma.txavail = 0;
	di->hnddma.txnobuf++;
	return (-1);
}

/** get the address of the var in order to change later */
uintptr
_dma_getvar(dma_info_t *di, const char *name)
{
	if (!strcmp(name, "&txavail"))
		return ((uintptr) &(di->hnddma.txavail));
	else if (!strcmp(name, "&rxavail"))
		return ((uintptr) &(di->rxavail));
	else {
		ASSERT(0);
	}
	return (0);
}

/**
 * Helper routine to do commit only operation on descriptors
 * Externally txcommit visible call
 */
void BCMFASTPATH
dma64_txcommit(dma_info_t *di)
{
	if (DMA_INDIRECT(di)) {
		dma_set_indqsel((hnddma_t *)di, FALSE);
	}

	dma64_txcommit_local(di);
}

static void BCMFASTPATH
dma64_dd_upd_64_from_struct(dma_info_t *di, dma64dd_t *ddring, dma64dd_t *dd, uint outidx)
{
	/* bit 63 is arleady set for host addresses by the caller */
	W_SM(&ddring[outidx].addrlow, BUS_SWAP32(dd->addrlow));
	W_SM(&ddring[outidx].addrhigh, BUS_SWAP32(dd->addrhigh));
	W_SM(&ddring[outidx].ctrl1, BUS_SWAP32(dd->ctrl1));
	W_SM(&ddring[outidx].ctrl2, BUS_SWAP32(dd->ctrl2));

	if (di->hnddma.dmactrlflags & DMA_CTRL_PEN) {
		if (DMA64_DD_PARITY(&ddring[outidx])) {
			W_SM(&ddring[outidx].ctrl2, BUS_SWAP32(dd->ctrl2 | D64_CTRL2_PARITY));
		}
	}
#if (defined(__ARM_ARCH_7A__) && defined(CA7)) || (defined(STB_SOC_WIFI))
	/* memory barrier before posting the descriptor */
	DMB();
#endif
}

/* Routine to post a descriptor. It requires the caller to send in partially formatted
 * information for the descriptor.
 */
int BCMFASTPATH
dma_txdesc(hnddma_t *dmah, dma64dd_t *dd, bool commit)
{
	uint16 txout;
	dma_info_t *di = DI_INFO(dmah);

	DMA_TRACE(("%s: dma_txdesc\n", di->name));
	txout = di->txout;

	/* return nonzero if out of tx descriptors */
	if (NEXTTXD(txout) == di->txin)
		goto outoftxd;

	/* fill in remaining bits in ctrl1 */
	if (di->burstsize_ctrl || (CHIPID(di->sih->chip) == BCM7271_CHIP_ID))
		dd->ctrl1 |= D64_CTRL1_NOTPCIE;

	/* dongle aqm desc need to have CO as the SOFD buffer comes from local
	 * memory.
	 */
	if (DMA_TRANSCOHERENT(di))
		dd->ctrl1 |= D64_CTRL1_COHERENT;
	if (txout == (di->ntxd - 1))
		dd->ctrl1 |= D64_CTRL1_EOT;

	DMA_TRACE(("%s: dma_txdesc Descriptor index = %d, dd->ctrl1 = 0x%x, dd->ctrl2 = 0x%x,"
		"dd->addrlow = 0x%x, dd->addrhigh = 0x%x \n", di->name, txout, dd->ctrl1,
		dd->ctrl2, dd->addrlow, dd->addrhigh));

	/* load the descriptor */
	dma64_dd_upd_64_from_struct(di, di->txd64, dd, txout);

	txout = NEXTTXD(txout);
	/* bump the descriptor index */
	di->txout = txout;

	/* If commit is set, write the DMA register to inform the DMA of the new descriptor */
	if (commit) {
		W_REG(di->osh, &di->d64txregs->ptr, di->xmtptrbase + I2B(txout, dma64dd_t));
	}

	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (0);

outoftxd:
	DMA_ERROR(("%s: dma_txdesc: out of txds !!!\n", di->name));
	di->hnddma.txavail = 0;
	di->hnddma.txnobuf++;
	return (-1);
}

/**
 * Reclaim next completed txd (txds if using chained buffers) in the range
 * specified and return associated packet.
 * If range is HNDDMA_RANGE_TRANSMITTED, reclaim descriptors that have be
 * transmitted as noted by the hardware "CurrDescr" pointer.
 * If range is HNDDMA_RANGE_TRANSFERED, reclaim descriptors that have be
 * transfered by the DMA as noted by the hardware "ActiveDescr" pointer.
 * If range is HNDDMA_RANGE_ALL, reclaim all txd(s) posted to the ring and
 * return associated packet regardless of the value of hardware pointers.
 */
void * BCMFASTPATH
dma64_getnexttxp(dma_info_t *di, txd_range_t range)
{
	uint16 start, end, i;
	uint16 active_desc;
	void *txp;

	DMA_TRACE(("%s: dma_getnexttxp %s\n", di->name,
	           (range == HNDDMA_RANGE_ALL) ? "all" :
	           ((range == HNDDMA_RANGE_TRANSMITTED) ? "transmitted" : "transfered")));

	/* if using indirect DMA access, then configure IndQSel */
	if (DMA_INDIRECT(di)) {
		dma_set_indqsel((hnddma_t *)di, FALSE);
	}

	if (di->ntxd == 0)
		return (NULL);

	txp = NULL;

	start = di->txin;
	if (range == HNDDMA_RANGE_ALL)
		end = di->txout;
	else {
		dma64regs_t *dregs = di->d64txregs;

		if (di->txin == di->xs0cd) {
			end = (uint16)(B2I(((R_REG(di->osh, &dregs->status0) & D64_XS0_CD_MASK) -
			      di->xmtptrbase) & D64_XS0_CD_MASK, dma64dd_t));
			di->xs0cd = end;
		} else
			end = di->xs0cd;

		if (range == HNDDMA_RANGE_TRANSFERED) {
			active_desc = (uint16)(R_REG(di->osh, &dregs->status1) & D64_XS1_AD_MASK);
			active_desc = (active_desc - di->xmtptrbase) & D64_XS1_AD_MASK;
			active_desc = B2I(active_desc, dma64dd_t);
			if (end != active_desc)
				end = PREVTXD(active_desc);
		}
	}

	if ((start == 0) && (end > di->txout))
		goto bogus;

	for (i = start; i != end && !txp; i = NEXTTXD(i)) {
		/* dma 64-bit */
		hnddma_seg_map_t *map = NULL;
		uint size, j, nsegs;

#if (!defined(__mips__) && !(defined(BCM47XX_CA9) || defined(STB))) || \
	defined(BCM_SECURE_DMA)
		dmaaddr_t pa;
		PHYSADDRLOSET(pa, (BUS_SWAP32(R_SM(&di->txd64[i].addrlow)) - di->dataoffsetlow));
		PHYSADDRHISET(pa, (BUS_SWAP32(R_SM(&di->txd64[i].addrhigh)) - di->dataoffsethigh));
#endif

		if (DMASGLIST_ENAB) {
			map = &di->txp_dmah[i];
			size = map->origsize;
			nsegs = map->nsegs;
			if (nsegs > (uint)NTXDACTIVE(i, end)) {
				di->xs0cd = i;
				break;
			}
		} else {
#if (!defined(__mips__) && !(defined(BCM47XX_CA9) || defined(STB))) || \
	defined(BCM_SECURE_DMA)
			size = (BUS_SWAP32(R_SM(&di->txd64[i].ctrl2)) & D64_CTRL2_BC_MASK);
#endif
			nsegs = 1;
		}

		for (j = nsegs; j > 0; j--) {
#if defined(DESCR_DEADBEEF)
			W_SM(&di->txd64[i].addrlow, 0xdeadbeef);
			W_SM(&di->txd64[i].addrhigh, 0xdeadbeef);
#endif

			txp = di->txp[i];
			di->txp[i] = NULL;
			if (j > 1)
				i = NEXTTXD(i);
		}
#ifdef BCM_SECURE_DMA
		SECURE_DMA_UNMAP(di->osh, pa, size, DMA_TX, NULL, NULL, &di->sec_cma_info_tx, 0);
#else
#if (!defined(__mips__) && !(defined(BCM47XX_CA9) || defined(STB)))
		DMA_UNMAP(di->osh, pa, size, DMA_TX, txp, map);
#endif
#endif /* BCM_SECURE_DMA */
	}

	di->txin = i;

	/* tx flow control */
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (txp);

bogus:
	DMA_NONE(("dma_getnexttxp: bogus curr: start %d end %d txout %d range %d\n",
	          start, end, di->txout, range));
	return (NULL);
}

/* Function to reclaim the next completed descriptors for DESCRIPTOR only DMA */
int BCMFASTPATH
dma_getnexttxdd(hnddma_t *dmah, txd_range_t range, uint32 *flags)
{
	dma_info_t *di = DI_INFO(dmah);
	uint16 end = 0;
	uint16 prev_txin = di->txin;

	DMA_TRACE(("  %s: dma_getnexttxdd %s\n", di->name,
	           (range == HNDDMA_RANGE_ALL) ? "all" :
	           ((range == HNDDMA_RANGE_TRANSMITTED) ? "transmitted" : "transfered")));

	/* The check below can be removed when/if new chips implement a DMA that does support
	 * use of the AD for Descriptor only DMAs, and that implementation to support the AD
	 * checking needs to be added below where the range is being set.
	 */
	if (range == HNDDMA_RANGE_TRANSFERED) {
		DMA_ERROR(("%s: dma_getnexttxdd: HNDDMA_RANGE_TRANSFERED is not valid range \n",
			di->name));
		ASSERT(range != HNDDMA_RANGE_TRANSFERED);
		return BCME_RANGE;
	}

	if (di->ntxd == 0) {
		DMA_ERROR(("%s: dma_getnexttxdd ntxd=0 \n", di->name));
		return BCME_ERROR;
	}

	/* if using indirect DMA access, then configure IndQSel */
	if (DMA_INDIRECT(di)) {
		dma_set_indqsel((hnddma_t *)di, FALSE);
	}

	if (range == HNDDMA_RANGE_ALL)
		end = di->txout;
	else {
		dma64regs_t *dregs = di->d64txregs;
		if (di->txin == di->xs0cd) {
			end = (uint16)(B2I(((R_REG(di->osh, &dregs->status0) & D64_XS0_CD_MASK) -
			      di->xmtptrbase) & D64_XS0_CD_MASK, dma64dd_t));
			di->xs0cd = end;
		} else
			end = di->xs0cd;
	}

	if (prev_txin == end)
		return BCME_NOTFOUND;

	*flags = BUS_SWAP32(R_SM(&di->txd64[prev_txin].ctrl1));

	di->txin = NEXTTXD(prev_txin);
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	DMA_TRACE(("%s: dma_getnexttxdd pre-txin=%d post-txin =%d, txavail = %d \n",
		di->name, prev_txin, di->txin, di->hnddma.txavail));
	return BCME_OK;
}

/* Function to get the next descriptor index to post */
uint16
dma_get_next_txd_idx(hnddma_t *di, bool txout)
{
	if (txout)
		return (DI_INFO(di))->txout;
	else
		return (DI_INFO(di))->txin;
}

/* Function to get the number of descriptors between start and txout/txin */
uint16
dma_get_txd_count(hnddma_t *dmah, uint16 start, bool txout)
{
	dma_info_t *di = DI_INFO(dmah);

	if (txout)
		return (NTXDACTIVE(start, di->txout));
	else
		return (NTXDACTIVE(start, di->txin));
}

/* Function to return the address of the descriptor with the given index */
uintptr
dma_get_txd_addr(hnddma_t *di, uint16 idx)
{
	dma_info_t *ddi = DI_INFO(di);
	return ((uintptr)(ddi->txd64 + idx));
}

/* Function to get the memory address of the buffer pointed to by the
 * descriptor #idx
 */
void
dma_get_txd_memaddr(hnddma_t *dmah, uint32 *addrlo, uint32 *addrhi, uint idx)
{
	dma_info_t *di = DI_INFO(dmah);
	/* get the memory address of the data buffer pointed by descriptor */
	*addrlo = BUS_SWAP32(R_SM(&di->txd64[idx].addrlow));
	*addrhi = BUS_SWAP32(R_SM(&di->txd64[idx].addrhigh));
}

#ifdef BCMLFRAG
/*
 * Sequentially program the pktdata(lfrag) - from TCM, followed by the
 * individual fragments from the HOST.
 */
static int BCMFASTPATH
dma64_txfast_lfrag(dma_info_t *di, void *p0, bool commit)
{
	void *p, *next;
	uchar *data;
	uint len;
	uint16 txout;
	uint32 flags = 0;
	dmaaddr_t pa;
	dma64addr_t pa64 = {0, 0};
	uint8 i = 0, j = 0;

	/* if using indirect DMA access, then configure IndQSel */
	if (DMA_INDIRECT(di)) {
		dma_set_indqsel((hnddma_t *)di, FALSE);
	}

	txout = di->txout;

	/*
	 * Lfrag - Program the descriptor for Lfrag data first before
	 * considering the individual fragments
	 */
	for (p = p0; p; p = next) {
		uint ftot = 0;
		uint nsegs = 1;

		next = PKTNEXT(di->osh, p);
		data = PKTDATA(di->osh, p);
		len  = PKTLEN(di->osh, p);

		if (PKTISFRAG(di->osh, p)) {
			ftot = PKTFRAGTOTNUM(di->osh, p);
		}

		if (len == 0) {
			/* Should not happen ideally unless this is a chained lfrag */
			goto program_frags;
		}
#ifndef BCM_SECURE_DMA
		pa = DMA_MAP(di->osh, data, len, DMA_TX, p, &di->txp_dmah[txout]);
#endif /* BCM_SECURE_DMA */
		{
			if ((nsegs+ftot) > (uint)(di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1))
				goto outoftxd;
		}

		for (j = 1; j <= nsegs; j++) {
			flags = 0;
			if ((p == p0) && (j == 1))
				flags |= D64_CTRL1_SOF;
			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;
			if (di->burstsize_ctrl || (CHIPID(di->sih->chip) == BCM7271_CHIP_ID))
				flags |= D64_CTRL1_NOTPCIE;

			if ((j == nsegs) && (ftot == 0) && (next == NULL))
				flags |= (D64_CTRL1_IOC | D64_CTRL1_EOF);

				dma64_dd_upd(di, di->txd64, pa, txout,
					&flags, len);

			ASSERT(di->txp[txout] == NULL);
			txout = NEXTTXD(txout);
		}

program_frags:
		/*
		 * Now, walk the chain of fragments in this lfrag allocating
		 * and initializing transmit descriptor entries.
		 */
		for (i = 1, j = 1; j <= ftot; i++, j++) {
			flags = 0;
			if (PKTFRAGISCHAINED(di->osh, i)) {
				 i = 1;
				 p = PKTNEXT(di->osh, p);
				 ASSERT(p != NULL);
				 next = PKTNEXT(di->osh, p);
			}

			len = PKTFRAGLEN(di->osh, p, i);

#ifdef BCM_DMAPAD
			if (DMAPADREQUIRED(di)) {
				len += PKTDMAPAD(di->osh, p);
			}
#endif /* BCM_DMAPAD */

			pa64.loaddr = PKTFRAGDATA_LO(di->osh, p, i);
			pa64.hiaddr = PKTFRAGDATA_HI(di->osh, p, i);

			if ((j == ftot) && (next == NULL))
				flags |= (D64_CTRL1_IOC | D64_CTRL1_EOF);
			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;

			/* War to handle 64 bit dma address for now */
			dma64_dd_upd_64_from_params(di, di->txd64, pa64, txout, &flags, len);

			ASSERT(di->txp[txout] == NULL);
			txout = NEXTTXD(txout);
		}
	}

	/* save the packet */
	di->txp[PREVTXD(txout)] = p0;

	/* bump the tx descriptor index */
	di->txout = txout;

	/* kick the chip */
	if (commit) {
		dma64_txcommit_local(di);
	}

	/* tx flow control */
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (0);

outoftxd:
	DMA_ERROR(("%s: %s: out of txds !!!\n", di->name, __FUNCTION__));
	di->hnddma.txavail = 0;
	di->hnddma.txnobuf++;
	return (-1);
}

static int BCMFASTPATH
dma64_txfast_sfd(dma_info_t *di, void *p0, bool commit)
{
	void *p, *next;
	uchar *data;
	uint len;
	uint16 txout;
	uint32 flags = 0;
	dmaaddr_t pa;
	dma64addr_t pa64 = {0, 0};
	uint8 i = 0, j = 0;
	uint8 *desc0, *desc1;
	uint16 len0, len1;

	txout = di->txout;

	/*
	 * Lfrag - Program the descriptor for Lfrag data first before
	 * considering the individual fragments
	 */
	for (p = p0; p; p = next) {
		uint ftot = 0;
		uint nsegs = 1;

		if (p == p0) {
			(*di->fn)(di->ctx, p, &desc0, &len0, &desc1, &len1);

			pa = DMA_MAP(di->osh, desc0, len0, DMA_TX,
					p, &di->txp_dmah[txout]);
			if (p == p0)
				flags |= D64_CTRL1_SOF;
			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;
			if (di->burstsize_ctrl)
				flags |= D64_CTRL1_NOTPCIE;

			dma64_dd_upd(di, di->txd64, pa, txout, &flags, len0);

			ASSERT(di->txp[txout] == NULL);
			txout = NEXTTXD(txout);

			flags = 0;
			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;
			if (di->burstsize_ctrl)
				flags |= D64_CTRL1_NOTPCIE;

			pa = DMA_MAP(di->osh, desc1, len1, DMA_TX,
					desc1, &di->txp_dmah[txout]);

			dma64_dd_upd(di, di->txd64, pa, txout, &flags, len1);

			ASSERT(di->txp[txout] == NULL);
			txout = NEXTTXD(txout);

			PKTPULL(di->osh, p, len0);
		}

		next = PKTNEXT(di->osh, p);
		data = PKTDATA(di->osh, p);
		len  = PKTLEN(di->osh, p);

		if (PKTISFRAG(di->osh, p)) {
			ftot = PKTFRAGTOTNUM(di->osh, p);
		}

		if (len == 0) {
			/* Should not happen ideally unless this is a chained lfrag */
			goto program_frags;
		}
#ifndef BCM_SECURE_DMA
		pa = DMA_MAP(di->osh, data, len, DMA_TX, p, &di->txp_dmah[txout]);
#endif /* BCM_SECURE_DMA */
		{
			if ((nsegs+ftot) > (uint)(di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1))
				goto outoftxd;
		}

		for (j = 1; j <= nsegs; j++) {
			flags = 0;

			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;
			if (di->burstsize_ctrl)
				flags |= D64_CTRL1_NOTPCIE;

			if ((j == nsegs) && (ftot == 0) && (next == NULL))
				flags |= (D64_CTRL1_IOC | D64_CTRL1_EOF);

				dma64_dd_upd(di, di->txd64, pa, txout,
					&flags, len);

			ASSERT(di->txp[txout] == NULL);
			txout = NEXTTXD(txout);
		}

		if (p == p0)
			PKTPUSH(di->osh, p, len0);

program_frags:
		/*
		 * Now, walk the chain of fragments in this lfrag allocating
		 * and initializing transmit descriptor entries.
		 */
		for (i = 1, j = 1; j <= ftot; i++, j++) {
			flags = 0;
			if (PKTFRAGISCHAINED(di->osh, i)) {
				 i = 1;
				 p = PKTNEXT(di->osh, p);
				 ASSERT(p != NULL);
				 next = PKTNEXT(di->osh, p);
			}

			len = PKTFRAGLEN(di->osh, p, i);

#ifdef BCM_DMAPAD
			if (DMAPADREQUIRED(di)) {
				len += PKTDMAPAD(di->osh, p);
			}
#endif /* BCM_DMAPAD */

			pa64.loaddr = PKTFRAGDATA_LO(di->osh, p, i);
			pa64.hiaddr = PKTFRAGDATA_HI(di->osh, p, i);

			if ((j == ftot) && (next == NULL))
				flags |= (D64_CTRL1_IOC | D64_CTRL1_EOF);
			if (txout == (di->ntxd - 1))
				flags |= D64_CTRL1_EOT;

			/* War to handle 64 bit dma address for now */
			dma64_dd_upd_64_from_params(di, di->txd64, pa64, txout, &flags, len);

			ASSERT(di->txp[txout] == NULL);
			txout = NEXTTXD(txout);
		}
	}

	/* save the packet */
	di->txp[PREVTXD(txout)] = p0;

	/* bump the tx descriptor index */
	di->txout = txout;

	/* kick the chip */
	if (commit) {
		dma64_txcommit_local(di);
	}

	/* tx flow control */
	di->hnddma.txavail = di->ntxd - NTXDACTIVE(di->txin, di->txout) - 1;

	return (0);

outoftxd:
	DMA_ERROR(("%s: dma_txfast: out of txds !!!\n", di->name));
	di->hnddma.txavail = 0;
	di->hnddma.txnobuf++;
	return (-1);
}
#endif /* BCMLFRAG */
