/*
 * Copyright (c) 2003 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please 
 * obtain a copy of the License at http://www.apple.com/publicsource and 
 * read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * Please see the License for the specific language governing rights and 
 * limitations under the License.
 */
 
/*
 * pkcs12BagAttrs.cpp : internal representation of P12 SafeBag 
 *                      attribute, OTHER THAN friendlyName and localKeyId.
 *					    This corresponds to a SecPkcs12AttrsRef at the
 *					    public API layer.
 */

#include "pkcs12BagAttrs.h"
#include "pkcs12Utils.h"
#include <SecurityNssAsn1/nssUtils.h>
#include <assert.h>
#include <CoreServices/../Frameworks/CarbonCore.framework/Headers/MacErrors.h>
#include <Security/utilities.h>

/* 
 * Copying constructor used by P12SafeBag during encoding
 */
P12BagAttrs::P12BagAttrs(
	const P12BagAttrs *otherAttrs,
	SecNssCoder &coder)
		: mAttrs(NULL), mCoder(coder)
{
	if(otherAttrs == NULL) {
		/* empty copy, done */
		return;
	}
	unsigned num = otherAttrs->numAttrs();
	reallocAttrs(num);
	for(unsigned dex=0; dex<num; dex++) {
		copyAttr(*otherAttrs->mAttrs[dex], *mAttrs[dex]);
	}
}

unsigned P12BagAttrs::numAttrs() const
{
	return nssArraySize((const void **)mAttrs);
}

NSS_Attribute *P12BagAttrs::getAttr(
	unsigned attrNum)
{
	assert(attrNum < numAttrs());
	return mAttrs[attrNum];
}
		

/*
 * Add an attr during decode.
 */
void P12BagAttrs::addAttr(
	const NSS_Attribute &attr)
{
	NSS_Attribute *newAttr = reallocAttrs(numAttrs() + 1);
	copyAttr(attr, *newAttr);
}

/*
 * Add an attr during encode.
 */
void P12BagAttrs::addAttr(
	const CFDataRef		attrOid,
	const CFArrayRef	attrValues)
{
	NSS_Attribute *newAttr = reallocAttrs(numAttrs() + 1);
	p12CfDataToCssm(attrOid, newAttr->attrType, mCoder);
	unsigned numVals = CFArrayGetCount(attrValues);
	newAttr->attrValue = (CSSM_DATA **)p12NssNullArray(numVals, mCoder);
	for(unsigned dex=0; dex<numVals; dex++) {
		CSSM_DATA *dstVal = (CSSM_DATA *)mCoder.malloc(sizeof(CSSM_DATA));
		newAttr->attrValue[dex] = dstVal;
		CFDataRef srcVal = (CFDataRef)CFArrayGetValueAtIndex(attrValues, dex);
		assert(CFGetTypeID(srcVal) == CFDataGetTypeID());
		p12CfDataToCssm(srcVal, *dstVal, mCoder);
	}
}

/* 
 * getter, public API version
 */
void P12BagAttrs::getAttr(
	unsigned			attrNum,
	CFDataRef			*attrOid,		// RETURNED
	CFArrayRef			*attrValues)	// RETURNED
{
	if(attrNum >= numAttrs()) {
		MacOSError::throwMe(paramErr);
	}
	NSS_Attribute *attr = mAttrs[attrNum];
	*attrOid = p12CssmDataToCf(attr->attrType);
	unsigned numVals = nssArraySize((const void **)attr->attrValue);
	if(numVals == 0) {
		/* maybe should return empty array...? */
		*attrValues = NULL;
		return;
	}
	CFMutableArrayRef vals = CFArrayCreateMutable(NULL, numVals, NULL);
	for(unsigned dex=0; dex<numVals; dex++) {
		CFDataRef val = p12CssmDataToCf(*attr->attrValue[dex]);
		CFArrayAppendValue(vals, val);
	}
	*attrValues = vals;
}

#pragma mark --- private methods ---

/*
 * Alloc/realloc attr array.
 * Returns ptr to new empty NSS_Attribute for insertion.
 */
NSS_Attribute *P12BagAttrs::reallocAttrs(
	size_t numNewAttrs)
{
	unsigned curSize = numAttrs();
	assert(numNewAttrs > curSize);
	NSS_Attribute **newAttrs = 
		(NSS_Attribute **)p12NssNullArray(numNewAttrs, mCoder);
	for(unsigned dex=0; dex<curSize; dex++) {
		newAttrs[dex] = mAttrs[dex];
	}
	mAttrs = newAttrs;
	
	/* allocate new NSS_Attributes */
	for(unsigned dex=curSize; dex<numNewAttrs; dex++) {
		mAttrs[dex] = mCoder.mallocn<NSS_Attribute>();
		memset(mAttrs[dex], 0, sizeof(NSS_Attribute));
	}
	return mAttrs[curSize];
}

void P12BagAttrs::copyAttr(
	const NSS_Attribute &src,
	NSS_Attribute &dst)
{
	mCoder.allocCopyItem(src.attrType, dst.attrType);
	unsigned numVals = nssArraySize((const void **)src.attrValue);
	dst.attrValue = (CSSM_DATA **)p12NssNullArray(numVals, mCoder);
	for(unsigned dex=0; dex<numVals; dex++) {
		CSSM_DATA *dstVal = mCoder.mallocn<CSSM_DATA>();
		memset(dstVal, 0, sizeof(CSSM_DATA));
		dst.attrValue[dex] = dstVal;
		mCoder.allocCopyItem(*src.attrValue[dex], *dstVal);
	}
}