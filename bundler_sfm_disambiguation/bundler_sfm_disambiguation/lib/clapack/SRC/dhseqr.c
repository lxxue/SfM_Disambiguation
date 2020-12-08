#include "f2c.h"
#include "blaswrap.h"

/* Table of constant values */

static doublereal c_b11 = 0.;
static doublereal c_b12 = 1.;
static integer c__12 = 12;
static integer c__2 = 2;
static integer c__49 = 49;

/* Subroutine */ int dhseqr_(char *job, char *compz, integer *n, integer *ilo, 
	 integer *ihi, doublereal *h__, integer *ldh, doublereal *wr, 
	doublereal *wi, doublereal *z__, integer *ldz, doublereal *work, 
	integer *lwork, integer *info)
{
    /* System generated locals */
    address a__1[2];
    integer h_dim1, h_offset, z_dim1, z_offset, i__1, i__2[2], i__3;
    doublereal d__1;
    char ch__1[2];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    integer i__;
    doublereal hl[2401]	/* was [49][49] */;
    integer kbot, nmin;
    extern logical lsame_(char *, char *);
    logical initz;
    doublereal workl[49];
    logical wantt, wantz;
    extern /* Subroutine */ int dlaqr0_(logical *, logical *, integer *, 
	    integer *, integer *, doublereal *, integer *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, integer *, integer *), dlahqr_(logical *, logical *, 
	     integer *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    integer *, integer *), dlacpy_(char *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, integer *), 
	    dlaset_(char *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    logical lquery;


/*  -- LAPACK driver routine (version 3.1) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     Purpose */
/*     ======= */

/*     DHSEQR computes the eigenvalues of a Hessenberg matrix H */
/*     and, optionally, the matrices T and Z from the Schur decomposition */
/*     H = Z T Z**T, where T is an upper quasi-triangular matrix (the */
/*     Schur form), and Z is the orthogonal matrix of Schur vectors. */

/*     Optionally Z may be postmultiplied into an input orthogonal */
/*     matrix Q so that this routine can give the Schur factorization */
/*     of a matrix A which has been reduced to the Hessenberg form H */
/*     by the orthogonal matrix Q:  A = Q*H*Q**T = (QZ)*T*(QZ)**T. */

/*     Arguments */
/*     ========= */

/*     JOB   (input) CHARACTER*1 */
/*           = 'E':  compute eigenvalues only; */
/*           = 'S':  compute eigenvalues and the Schur form T. */

/*     COMPZ (input) CHARACTER*1 */
/*           = 'N':  no Schur vectors are computed; */
/*           = 'I':  Z is initialized to the unit matrix and the matrix Z */
/*                   of Schur vectors of H is returned; */
/*           = 'V':  Z must contain an orthogonal matrix Q on entry, and */
/*                   the product Q*Z is returned. */

/*     N     (input) INTEGER */
/*           The order of the matrix H.  N .GE. 0. */

/*     ILO   (input) INTEGER */
/*     IHI   (input) INTEGER */
/*           It is assumed that H is already upper triangular in rows */
/*           and columns 1:ILO-1 and IHI+1:N. ILO and IHI are normally */
/*           set by a previous call to DGEBAL, and then passed to DGEHRD */
/*           when the matrix output by DGEBAL is reduced to Hessenberg */
/*           form. Otherwise ILO and IHI should be set to 1 and N */
/*           respectively.  If N.GT.0, then 1.LE.ILO.LE.IHI.LE.N. */
/*           If N = 0, then ILO = 1 and IHI = 0. */

/*     H     (input/output) DOUBLE PRECISION array, dimension (LDH,N) */
/*           On entry, the upper Hessenberg matrix H. */
/*           On exit, if INFO = 0 and JOB = 'S', then H contains the */
/*           upper quasi-triangular matrix T from the Schur decomposition */
/*           (the Schur form); 2-by-2 diagonal blocks (corresponding to */
/*           complex conjugate pairs of eigenvalues) are returned in */
/*           standard form, with H(i,i) = H(i+1,i+1) and */
/*           H(i+1,i)*H(i,i+1).LT.0. If INFO = 0 and JOB = 'E', the */
/*           contents of H are unspecified on exit.  (The output value of */
/*           H when INFO.GT.0 is given under the description of INFO */
/*           below.) */

/*           Unlike earlier versions of DHSEQR, this subroutine may */
/*           explicitly H(i,j) = 0 for i.GT.j and j = 1, 2, ... ILO-1 */
/*           or j = IHI+1, IHI+2, ... N. */

/*     LDH   (input) INTEGER */
/*           The leading dimension of the array H. LDH .GE. max(1,N). */

/*     WR    (output) DOUBLE PRECISION array, dimension (N) */
/*     WI    (output) DOUBLE PRECISION array, dimension (N) */
/*           The real and imaginary parts, respectively, of the computed */
/*           eigenvalues. If two eigenvalues are computed as a complex */
/*           conjugate pair, they are stored in consecutive elements of */
/*           WR and WI, say the i-th and (i+1)th, with WI(i) .GT. 0 and */
/*           WI(i+1) .LT. 0. If JOB = 'S', the eigenvalues are stored in */
/*           the same order as on the diagonal of the Schur form returned */
/*           in H, with WR(i) = H(i,i) and, if H(i:i+1,i:i+1) is a 2-by-2 */
/*           diagonal block, WI(i) = sqrt(-H(i+1,i)*H(i,i+1)) and */
/*           WI(i+1) = -WI(i). */

/*     Z     (input/output) DOUBLE PRECISION array, dimension (LDZ,N) */
/*           If COMPZ = 'N', Z is not referenced. */
/*           If COMPZ = 'I', on entry Z need not be set and on exit, */
/*           if INFO = 0, Z contains the orthogonal matrix Z of the Schur */
/*           vectors of H.  If COMPZ = 'V', on entry Z must contain an */
/*           N-by-N matrix Q, which is assumed to be equal to the unit */
/*           matrix except for the submatrix Z(ILO:IHI,ILO:IHI). On exit, */
/*           if INFO = 0, Z contains Q*Z. */
/*           Normally Q is the orthogonal matrix generated by DORGHR */
/*           after the call to DGEHRD which formed the Hessenberg matrix */
/*           H. (The output value of Z when INFO.GT.0 is given under */
/*           the description of INFO below.) */

/*     LDZ   (input) INTEGER */
/*           The leading dimension of the array Z.  if COMPZ = 'I' or */
/*           COMPZ = 'V', then LDZ.GE.MAX(1,N).  Otherwize, LDZ.GE.1. */

/*     WORK  (workspace/output) DOUBLE PRECISION array, dimension (LWORK) */
/*           On exit, if INFO = 0, WORK(1) returns an estimate of */
/*           the optimal value for LWORK. */

/*     LWORK (input) INTEGER */
/*           The dimension of the array WORK.  LWORK .GE. max(1,N) */
/*           is sufficient, but LWORK typically as large as 6*N may */
/*           be required for optimal performance.  A workspace query */
/*           to determine the optimal workspace size is recommended. */

/*           If LWORK = -1, then DHSEQR does a workspace query. */
/*           In this case, DHSEQR checks the input parameters and */
/*           estimates the optimal workspace size for the given */
/*           values of N, ILO and IHI.  The estimate is returned */
/*           in WORK(1).  No error message related to LWORK is */
/*           issued by XERBLA.  Neither H nor Z are accessed. */


/*     INFO  (output) INTEGER */
/*             =  0:  successful exit */
/*           .LT. 0:  if INFO = -i, the i-th argument had an illegal */
/*                    value */
/*           .GT. 0:  if INFO = i, DHSEQR failed to compute all of */
/*                the eigenvalues.  Elements 1:ilo-1 and i+1:n of WR */
/*                and WI contain those eigenvalues which have been */
/*                successfully computed.  (Failures are rare.) */

/*                If INFO .GT. 0 and JOB = 'E', then on exit, the */
/*                remaining unconverged eigenvalues are the eigen- */
/*                values of the upper Hessenberg matrix rows and */
/*                columns ILO through INFO of the final, output */
/*                value of H. */

/*                If INFO .GT. 0 and JOB   = 'S', then on exit */

/*           (*)  (initial value of H)*U  = U*(final value of H) */

/*                where U is an orthogonal matrix.  The final */
/*                value of H is upper Hessenberg and quasi-triangular */
/*                in rows and columns INFO+1 through IHI. */

/*                If INFO .GT. 0 and COMPZ = 'V', then on exit */

/*                  (final value of Z)  =  (initial value of Z)*U */

/*                where U is the orthogonal matrix in (*) (regard- */
/*                less of the value of JOB.) */

/*                If INFO .GT. 0 and COMPZ = 'I', then on exit */
/*                      (final value of Z)  = U */
/*                where U is the orthogonal matrix in (*) (regard- */
/*                less of the value of JOB.) */

/*                If INFO .GT. 0 and COMPZ = 'N', then Z is not */
/*                accessed. */

/*     ================================================================ */
/*             Default values supplied by */
/*             ILAENV(ISPEC,'DHSEQR',JOB(:1)//COMPZ(:1),N,ILO,IHI,LWORK). */
/*             It is suggested that these defaults be adjusted in order */
/*             to attain best performance in each particular */
/*             computational environment. */

/*            ISPEC=1:  The DLAHQR vs DLAQR0 crossover point. */
/*                      Default: 75. (Must be at least 11.) */

/*            ISPEC=2:  Recommended deflation window size. */
/*                      This depends on ILO, IHI and NS.  NS is the */
/*                      number of simultaneous shifts returned */
/*                      by ILAENV(ISPEC=4).  (See ISPEC=4 below.) */
/*                      The default for (IHI-ILO+1).LE.500 is NS. */
/*                      The default for (IHI-ILO+1).GT.500 is 3*NS/2. */

/*            ISPEC=3:  Nibble crossover point. (See ILAENV for */
/*                      details.)  Default: 14% of deflation window */
/*                      size. */

/*            ISPEC=4:  Number of simultaneous shifts, NS, in */
/*                      a multi-shift QR iteration. */

/*                      If IHI-ILO+1 is ... */

/*                      greater than      ...but less    ... the */
/*                      or equal to ...      than        default is */

/*                           1               30          NS -   2(+) */
/*                          30               60          NS -   4(+) */
/*                          60              150          NS =  10(+) */
/*                         150              590          NS =  ** */
/*                         590             3000          NS =  64 */
/*                        3000             6000          NS = 128 */
/*                        6000             infinity      NS = 256 */

/*                  (+)  By default some or all matrices of this order */
/*                       are passed to the implicit double shift routine */
/*                       DLAHQR and NS is ignored.  See ISPEC=1 above */
/*                       and comments in IPARM for details. */

/*                       The asterisks (**) indicate an ad-hoc */
/*                       function of N increasing from 10 to 64. */

/*            ISPEC=5:  Select structured matrix multiply. */
/*                      (See ILAENV for details.) Default: 3. */

/*     ================================================================ */
/*     Based on contributions by */
/*        Karen Braman and Ralph Byers, Department of Mathematics, */
/*        University of Kansas, USA */

/*     ================================================================ */
/*     References: */
/*       K. Braman, R. Byers and R. Mathias, The Multi-Shift QR */
/*       Algorithm Part I: Maintaining Well Focused Shifts, and Level 3 */
/*       Performance, SIAM Journal of Matrix Analysis, volume 23, pages */
/*       929--947, 2002. */

/*       K. Braman, R. Byers and R. Mathias, The Multi-Shift QR */
/*       Algorithm Part II: Aggressive Early Deflation, SIAM Journal */
/*       of Matrix Analysis, volume 23, pages 948--973, 2002. */

/*     ================================================================ */
/*     .. Parameters .. */

/*     ==== Matrices of order NTINY or smaller must be processed by */
/*     .    DLAHQR because of insufficient subdiagonal scratch space. */
/*     .    (This is a hard limit.) ==== */

/*     ==== NL allocates some local workspace to help small matrices */
/*     .    through a rare DLAHQR failure.  NL .GT. NTINY = 11 is */
/*     .    required and NL .LE. NMIN = ILAENV(ISPEC=1,...) is recom- */
/*     .    mended.  (The default value of NMIN is 75.)  Using NL = 49 */
/*     .    allows up to six simultaneous shifts and a 16-by-16 */
/*     .    deflation window.  ==== */

/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     ==== Decode and check the input parameters. ==== */

    /* Parameter adjustments */
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    --wr;
    --wi;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    --work;

    /* Function Body */
    wantt = lsame_(job, "S");
    initz = lsame_(compz, "I");
    wantz = initz || lsame_(compz, "V");
    work[1] = (doublereal) max(1,*n);
    lquery = *lwork == -1;

    *info = 0;
    if (! lsame_(job, "E") && ! wantt) {
	*info = -1;
    } else if (! lsame_(compz, "N") && ! wantz) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ilo < 1 || *ilo > max(1,*n)) {
	*info = -4;
    } else if (*ihi < min(*ilo,*n) || *ihi > *n) {
	*info = -5;
    } else if (*ldh < max(1,*n)) {
	*info = -7;
    } else if (*ldz < 1 || wantz && *ldz < max(1,*n)) {
	*info = -11;
    } else if (*lwork < max(1,*n) && ! lquery) {
	*info = -13;
    }

    if (*info != 0) {

/*        ==== Quick return in case of invalid argument. ==== */

	i__1 = -(*info);
	xerbla_("DHSEQR", &i__1);
	return 0;

    } else if (*n == 0) {

/*        ==== Quick return in case N = 0; nothing to do. ==== */

	return 0;

    } else if (lquery) {

/*        ==== Quick return in case of a workspace query ==== */

	dlaqr0_(&wantt, &wantz, n, ilo, ihi, &h__[h_offset], ldh, &wr[1], &wi[
		1], ilo, ihi, &z__[z_offset], ldz, &work[1], lwork, info);
/*        ==== Ensure reported workspace size is backward-compatible with */
/*        .    previous LAPACK versions. ==== */
/* Computing MAX */
	d__1 = (doublereal) max(1,*n);
	work[1] = max(d__1,work[1]);
	return 0;

    } else {

/*        ==== copy eigenvalues isolated by DGEBAL ==== */

	i__1 = *ilo - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    wr[i__] = h__[i__ + i__ * h_dim1];
	    wi[i__] = 0.;
/* L10: */
	}
	i__1 = *n;
	for (i__ = *ihi + 1; i__ <= i__1; ++i__) {
	    wr[i__] = h__[i__ + i__ * h_dim1];
	    wi[i__] = 0.;
/* L20: */
	}

/*        ==== Initialize Z, if requested ==== */

	if (initz) {
	    dlaset_("A", n, n, &c_b11, &c_b12, &z__[z_offset], ldz)
		    ;
	}

/*        ==== Quick return if possible ==== */

	if (*ilo == *ihi) {
	    wr[*ilo] = h__[*ilo + *ilo * h_dim1];
	    wi[*ilo] = 0.;
	    return 0;
	}

/*        ==== DLAHQR/DLAQR0 crossover point ==== */

/* Writing concatenation */
	i__2[0] = 1, a__1[0] = job;
	i__2[1] = 1, a__1[1] = compz;
	s_cat(ch__1, a__1, i__2, &c__2, (ftnlen)2);
	nmin = ilaenv_(&c__12, "DHSEQR", ch__1, n, ilo, ihi, lwork);
	nmin = max(11,nmin);

/*        ==== DLAQR0 for big matrices; DLAHQR for small ones ==== */

	if (*n > nmin) {
	    dlaqr0_(&wantt, &wantz, n, ilo, ihi, &h__[h_offset], ldh, &wr[1], 
		    &wi[1], ilo, ihi, &z__[z_offset], ldz, &work[1], lwork, 
		    info);
	} else {

/*           ==== Small matrix ==== */

	    dlahqr_(&wantt, &wantz, n, ilo, ihi, &h__[h_offset], ldh, &wr[1], 
		    &wi[1], ilo, ihi, &z__[z_offset], ldz, info);

	    if (*info > 0) {

/*              ==== A rare DLAHQR failure!  DLAQR0 sometimes succeeds */
/*              .    when DLAHQR fails. ==== */

		kbot = *info;

		if (*n >= 49) {

/*                 ==== Larger matrices have enough subdiagonal scratch */
/*                 .    space to call DLAQR0 directly. ==== */

		    dlaqr0_(&wantt, &wantz, n, ilo, &kbot, &h__[h_offset], 
			    ldh, &wr[1], &wi[1], ilo, ihi, &z__[z_offset], 
			    ldz, &work[1], lwork, info);

		} else {

/*                 ==== Tiny matrices don't have enough subdiagonal */
/*                 .    scratch space to benefit from DLAQR0.  Hence, */
/*                 .    tiny matrices must be copied into a larger */
/*                 .    array before calling DLAQR0. ==== */

		    dlacpy_("A", n, n, &h__[h_offset], ldh, hl, &c__49);
		    hl[*n + 1 + *n * 49 - 50] = 0.;
		    i__1 = 49 - *n;
		    dlaset_("A", &c__49, &i__1, &c_b11, &c_b11, &hl[(*n + 1) *
			     49 - 49], &c__49);
		    dlaqr0_(&wantt, &wantz, &c__49, ilo, &kbot, hl, &c__49, &
			    wr[1], &wi[1], ilo, ihi, &z__[z_offset], ldz, 
			    workl, &c__49, info);
		    if (wantt || *info != 0) {
			dlacpy_("A", n, n, hl, &c__49, &h__[h_offset], ldh);
		    }
		}
	    }
	}

/*        ==== Clear out the trash, if necessary. ==== */

	if ((wantt || *info != 0) && *n > 2) {
	    i__1 = *n - 2;
	    i__3 = *n - 2;
	    dlaset_("L", &i__1, &i__3, &c_b11, &c_b11, &h__[h_dim1 + 3], ldh);
	}

/*        ==== Ensure reported workspace size is backward-compatible with */
/*        .    previous LAPACK versions. ==== */

/* Computing MAX */
	d__1 = (doublereal) max(1,*n);
	work[1] = max(d__1,work[1]);
    }

/*     ==== End of DHSEQR ==== */

    return 0;
} /* dhseqr_ */