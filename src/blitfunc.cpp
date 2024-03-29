#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "uae.h"
#include "options.h"
#include "custom.h"
#include "memory.h"
#include "blitter.h"
#include "blitfunc.h"
#include "debug_uae4all.h"
#include "include/memory.h"

/*
	gno: optimized..
	notaz: too :)
*/

void blitdofast_0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
unsigned int i,j,hblitsize,bltdmod;
if (!ptd || !b->vblitsize || !b->hblitsize) return;
hblitsize = b->hblitsize;
bltdmod = b->bltdmod;
j = b->vblitsize;
do {
	i = hblitsize;
	do {
		CHIPMEM_WPUT (ptd, 0);
		ptd += 2;
	} while (--i);
	ptd += bltdmod;
} while (--j);
}
void blitdofast_desc_0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
unsigned int i,j,hblitsize,bltdmod;
if (!ptd || !b->vblitsize || !b->hblitsize) return;
hblitsize = b->hblitsize;
bltdmod = b->bltdmod;
j = b->vblitsize;
do {
	i = hblitsize;
	do {
		CHIPMEM_WPUT (ptd, 0);
		ptd -= 2;
	} while (--i);
	ptd -= bltdmod;
} while (--j);
}
void blitdofast_a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 hblitsize = b->hblitsize;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - hblitsize;
if (!b->vblitsize || !hblitsize) return;
for (j = b->vblitsize; j--;) {
	for (i = hblitsize; i--;) {
		uae_u32 bltadat, srca, dstd;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((~srca & srcc));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 hblitsize = b->hblitsize;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((~srca & srcc));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_2a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc & ~(srca & srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_2a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc & ~(srca & srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_30 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srca & ~srcb));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_30 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srca & ~srcb));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_3a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcb ^ (srca | (srcb ^ srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_3a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcb ^ (srca | (srcb ^ srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_3c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srca ^ srcb));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_3c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srca ^ srcb));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_4a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & (srcb | srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_4a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & (srcb | srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_6a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_6a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_8a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc & (~srca | srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_8a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc & (~srca | srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_8c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcb & (~srca | srcc)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_8c (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcb & (~srca | srcc)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_9a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & ~srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_9a (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & ~srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_a8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc & (srca | srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_a8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc & (srca | srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_aa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		dstd = (srcc);
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_aa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		dstd = (srcc);
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_b1 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = (~(srca ^ (srcc | (srca ^ srcb))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_b1 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = (~(srca ^ (srcc | (srca ^ srcb))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_ca (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & (srcb ^ srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_ca (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc ^ (srca & (srcb ^ srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_cc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		dstd = (srcb);
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_cc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		dstd = (srcb);
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_d8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srca ^ (srcc & (srca ^ srcb))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_d8 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srca ^ (srcc & (srca ^ srcb))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_e2 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc ^ (srcb & (srca ^ srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_e2 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc ^ (srcb & (srca ^ srcc))));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_ea (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srcc | (srca & srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_ea (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srcc | (srca & srcb)));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_f0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = (srca);
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptd) ptd += b->bltdmod;
}
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_f0 (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = (srca);
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptd) ptd -= b->bltdmod;
}
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_fa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc += 2; }
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srca | srcc));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptc) ptc += b->bltcmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltcdat = srcc;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_fa (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptc) { srcc = CHIPMEM_WGET (ptc); ptc -= 2; }
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srca | srcc));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptc) ptc -= b->bltcmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
}
void blitdofast_fc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
register unsigned int i,j;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;

		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		dstd = ((srca | srcb));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd += 2; }
	}
	if (pta) pta += b->bltamod;
	if (ptb) ptb += b->bltbmod;
	if (ptd) ptd += b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
void blitdofast_desc_fc (uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *_GCCRES_ b)
{
uae_u32 totald = 0;
register unsigned int i,j;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 dstd=0;
uae_u32 *blit_masktable_p = blit_masktable + BLITTER_MAX_WORDS - b->hblitsize;
for (j = b->vblitsize; j--;) {
	for (i = b->hblitsize; i--;) {
		uae_u32 bltadat, srca;
		if (ptb) {
			uae_u32 bltbdat = CHIPMEM_WGET (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (pta) { bltadat = CHIPMEM_WGET (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable_p[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		dstd = ((srca | srcb));
		totald |= dstd;
		if (ptd) { CHIPMEM_WPUT (ptd, dstd); ptd -= 2; }
	}
	if (pta) pta -= b->bltamod;
	if (ptb) ptb -= b->bltbmod;
	if (ptd) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
if ((totald<<16) != 0) b->blitzero = 0;
}
