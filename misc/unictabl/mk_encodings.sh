#!/bin/sh
# CVS-ID: $Id$


echo "  * compiling C source generator..."

cc mk_ctable.c -o mk_ctable

echo "  * writing copyright info..."

echo "

/*
 *  This file is #included by encconv.cpp
 *
 *  CVS-ID: \$Id\$
 *
 *  *** ***  CAUTION! *** ***
 *  Do not modify this file by hand! It is generated by shell
 *  script \$(WXWIN)/misc/unictabl/regenerate
 *
 *  Parts of this file are based on tables published by Unicode, Inc.
 *  Original tables are freely available at
 *      ftp://ftp.unicode.org/Public/MAPPINGS
 *
 *  Original copyright info as present in mapping tables follows:
 *
 *
 *  Copyright (c) 1991-1999 Unicode, Inc.  All Rights reserved.
 *
 *  This file is provided as-is by Unicode, Inc. (The Unicode Consortium).
 *  No claims are made as to fitness for any particular purpose.  No
 *  warranties of any kind are expressed or implied.  The recipient
 *  agrees to determine applicability of information provided.  If this
 *  file has been provided on optical media by Unicode, Inc., the sole
 *  remedy for any claim will be exchange of defective media within 90
 *  days of receipt.
 *
 *  Unicode, Inc. hereby grants the right to freely use the information
 *  supplied in this file in the creation of products supporting the
 *  Unicode Standard, and to make copies of this file in any form for
 *  internal or external distribution as long as this notice remains
 *  attached.
 */

" > unictabl.inc


echo "  * creating C tables..."

all_encodings=""

for i in mappings/*.TXT ; do

    enc=`echo $i | cut -c10- | tr - _ | sed 's/\.TXT//g' |\
sed 's/8859_\(.*\)/ISO8859_\1/g
s/KOI8_R/KOI8/g'`
    cat $i | sed -n '/^0x...0x..../p' | cut -f1,2 | \
             ./mk_ctable $i $enc >> unictabl.inc
    all_encodings="$all_encodings $enc"

done

rm -f mk_ctable


echo "  * adding fallback..."

echo "




/*
 *
 * Unicode to 7bit ASCII fallback
 * (for use with wxRECODE_SUBSTITUTE recoding mode)
 *
 */

static struct {
    wxUint16 c  /*code*/;
    wxUint8  s /*inaccurate substitution*/;
} encoding_unicode_fallback[] = {
" >> unictabl.inc

cat Fallbacks | while read i ; do
    code=`echo "$i" | cut -f1`
    subs=`echo "$i" | cut -f2 | cut -c1-2,5-6`
    echo "    {$code, $subs}," >> unictabl.inc
done
echo "    {0, 0}" >> unictabl.inc
echo " };" >> unictabl.inc
echo "
static unsigned encoding_unicode_fallback_count = "`cat Fallbacks | wc -l`";" >> unictabl.inc


echo "  * adding reference table..."


echo "



/*
 *
 * Table of all supported encodings:
 *
 */

static struct {
    wxFontEncoding encoding; // encoding identifier
    wxUint16 *table;         // 8bit to unicode table
} encodings_list[] = {
" >> unictabl.inc

for i in $all_encodings ; do
    echo "    { wxFONTENCODING_$i, encoding_table__$i}," >> unictabl.inc
done

echo "    {wxFONTENCODING_MAX /*anything*/, NULL}" >> unictabl.inc
echo " };" >> unictabl.inc
