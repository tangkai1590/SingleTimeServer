

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed May 16 16:00:10 2018
 */
/* Compiler settings for NtpServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __NtpServer_h_h__
#define __NtpServer_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INtpServer_FWD_DEFINED__
#define __INtpServer_FWD_DEFINED__
typedef interface INtpServer INtpServer;

#endif 	/* __INtpServer_FWD_DEFINED__ */


#ifndef __NtpServer_FWD_DEFINED__
#define __NtpServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class NtpServer NtpServer;
#else
typedef struct NtpServer NtpServer;
#endif /* __cplusplus */

#endif 	/* __NtpServer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __NtpServer_LIBRARY_DEFINED__
#define __NtpServer_LIBRARY_DEFINED__

/* library NtpServer */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_NtpServer;

#ifndef __INtpServer_DISPINTERFACE_DEFINED__
#define __INtpServer_DISPINTERFACE_DEFINED__

/* dispinterface INtpServer */
/* [uuid] */ 


EXTERN_C const IID DIID_INtpServer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7D3339F8-DC8C-46D5-B834-D4428498636D")
    INtpServer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct INtpServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INtpServer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INtpServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INtpServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INtpServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INtpServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INtpServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INtpServer * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } INtpServerVtbl;

    interface INtpServer
    {
        CONST_VTBL struct INtpServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INtpServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INtpServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INtpServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INtpServer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define INtpServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define INtpServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define INtpServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __INtpServer_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_NtpServer;

#ifdef __cplusplus

class DECLSPEC_UUID("4203E43E-C4A3-4D52-9D30-81E566B1CDD0")
NtpServer;
#endif
#endif /* __NtpServer_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


