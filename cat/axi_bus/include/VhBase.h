/*==========================================

 This file and related documentation are
 proprietary and confidential to Siemens
         Copyright Siemens 2021

===========================================*/

/************************************************************************

    File             : VhBase.h

    Author           : Rudra,Manish 

    Date Started     : 15th May, 1995

    Revision History :

    Classes : VhFileName, VhBase, VhNamed, VhDecl

************************************************************************/


#ifndef __VHBASE__H
#define __VHBASE__H

#ifndef _preprocess_for_cgen
#include <vector>
#endif

// needed for storing both line number and column number into one unsigned
#define NUM_BITS_LINE_NO    22
#define NUM_BITS_COLM_NO    10

typedef unsigned int unsInt;

extern int     vha_getActualLineNo(unsInt lcolNo);
extern int     vha_getActualColNo(unsInt lcolNo);
extern unsigned vhGi_protectedRegionCounter;
extern int     vhGi_InsideDecryptionEnvelope;
extern VhBoolean  vhGi_parsingEinput;

#ifndef _preprocess_for_cgen

// assertion checks
#ifndef JAG_ASSERT //[
#ifndef NDEBUG //[
#include <assert.h>
#ifdef _LINUX //[
#include <stdlib.h>
// Stopping Asserts with environmental variable "RTLC_STOP_ASSERT"
// Currently only in Linux
#ifdef ASSERTONDEBUG
static const char* sIsAssertStop = getenv("RTLC_STOP_ASSERT");
#define JAG_ASSERT(x) assert((x) || (sIsAssertStop && printf("\n******  Error: Assert failed at line no %d in file %s ******\n\n",__LINE__,__FILE__)))
#else
#define JAG_ASSERT(x) assert(x)
#endif
#else //][
#define JAG_ASSERT(x) assert(x)
#endif //]
#else //][
#include <assert.h>
#define JAG_ASSERT(x) assert(x)
#endif //]
#endif //]

#endif


/*
    Class VhFileName is created only once for the file
    to be analyzed. Keeps the name of the file as a
    character string. During analysis, we have a global
    object for this class, called vhGi_currentFile.
*/

class VhFileName
{
    unsigned  objType;
    char* fileName;
    char* absFileName;

public:
    VhFileName() {};
    VhFileName(char* fname);
    ~VhFileName();

    char* getFileName() { return fileName; }
    char* getAbsFileName() { return absFileName; }

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s);
    void *operator new(size_t s,int flag);
    void operator delete(void *p);
#if (defined(__alpha) && !defined(__GNUC__))
    void operator delete(void *p,int flag);
#endif
};


/*
    VhBase will be defined as the top level class for
    all JAGUAR objects. All JAGUAR object model classes
    are directly or indirectly derived from VhBase.

    This allows us to store the line number for every
    object. In addition it also maintains a pointer to
    the scope in which this object occurs.

    The member objType defines what actually the object is.

    This class has overloaded the new and delete operators
    to enable the usage of JAGUAR internal memory manager
    and also to facilitate the dump and restore routines
    for the VHDL libraries. These 2 functions will be used
    whenever a VhBase or any object derived from VhBase is
    created.
*/
class VhBase
{
private:
    unsigned objType;
    /* the rightmost 31 bits of the above field will store
       the enum value VhObjType.
       31st bit - is used to mark if the node is encrypted i.e, belongs to
                  a protected region
       30th bit - is used to mark if the node is a Jaguar node. So this bit
                  will get set always. (PR_13133, 13123)
                  JAG-2813: We need this bit setting for some other
                  classes which are not derived from VHBASE also. 
                  If we use any other bit in future then we need to inform MVV, so
                  that they can change their MASK accordingly.
       29th Bit - check if VhTypeDef is of floating/physical type

       Note: 
             To check whether the 30th bit is set or not, masking out 
             1st 10bit for ObjType and 31st and 29th for other info,
             MVV should  mask now with (0x5ffffc00).

             To use any other bit(s) in future, you need to modify
             the member functions getObjectType and setObjectType.
             Also, the value of the mask OBJTYPE_MASK in the generated 
             files dump.cxx, restore.cxx and decom.cxx need to be updated
             by modifying the corresponding fprintf statements in
             utils/c-gen/bif_src_gen/ir.y and utils/c-gen/dswalker/dsw.y
             for dump/restore mechanism and browse utility respectively.
    */

private:
    /* this field will store the line number as well as column
       number info for each object. Of the 32 bits, 21 bits are
       being used for the actual line number while the top 11
       bits are used for the actual column number. All such
       manipulation is performed in the lexer.
    */
    unsigned LineNumber;



    /* Similar to the start line no, following field will hold
    ** the start and end line number.
    */

    unsigned _nEndLineCol ;

     
#ifdef _DFT_HDLENGINE_
    unsigned mObjectId;
#endif    
    VhBase* scope;

public :
#ifdef _DFT_HDLENGINE_
    VhBase();
#else    
    VhBase() {};
#endif
    /* Added during the support of JAG-3044 */

    VhBase(const VhBase &A_Bs);
    VhBase(VhBase *,char *, unsigned, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    VhBase(char* , unsigned , unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhBase(){}

    /* Function to set and get the start/end line no. */

    /* 
    ** START LINE NO - This is just a wrapper over the existing functions.
    ** The existing functions by default returns the start line no and start
    ** column number. This wrapper just have been written for the proper
    ** nomenclature.
    */

    inline unsigned getStartLineNumber() const 
    {
        return LineNumber;
    }
    
    void setStartLineNumber(unsigned A_LineNoBegin)
    {
        this->setLineNumber(A_LineNoBegin);
    }

    inline int getStartActLineNumber() const 
    {
        return this->getActLinNumber();
    }

    inline int getStartActColNumber() const 
    {
        return this->getActColNumber() ;
    }

    /* END LINE NO */

    inline unsigned getEndLineNumber() const 
    {
        return this->_nEndLineCol ;
    }
    
    void setEndLineNumber(unsigned A_LineNoEnd)
    {
        this->_nEndLineCol = A_LineNoEnd;
    }

    inline int getEndActLineNumber() const 
    {
        return vha_getActualLineNo(this->_nEndLineCol);
    }

    inline int getEndActColNumber() const 
    {
        return vha_getActualColNo(this->_nEndLineCol);
    }

    /* Function to set and get the end line no. */

    void setLineNumber(unsigned lineNumber)
    {
        this->LineNumber=lineNumber;
    }

#ifdef _DFT_HDLENGINE_
    unsigned ObjId() const { return mObjectId; }
    void setObjId(unsigned objId) { mObjectId = objId; }
#endif    

    /* Foll. 2 funcs. extract the actual line and column no. of any object
       from the member LineNumber, which uses 21 bits for the actual line
       number and the first 11 (31-21) bits store the column no. */

    int getActLinNumber() const 
    {
        return vha_getActualLineNo(this->LineNumber);
    }

    int getActColNumber() const 
    {
        return vha_getActualColNo(this->LineNumber);
    }
    char* getFileName();
    char* getAbsFileName();

    VhObjType getObjectType() const {
        return (VhObjType)(this->objType & 0x1fffffff);
    }     

    void setObjectType(VhObjType type) {
      if ((vhGi_protectedRegionCounter > 0)    || 
          (vhGi_InsideDecryptionEnvelope > 0)  || 
          (vhGi_parsingEinput == VH_TRUE)       ) {
        objType = ((unsigned)type | 0xC0000000); // Make 31st & 30th bit high.
      } else {
        objType = ((unsigned)type | 0x40000000); // Make 30th bit high.
      }
    }     

    VhBoolean getIfInsideProtected() const {
        return ((((objType >> 31) & 0x00000001 ) == 1) ? VH_TRUE : VH_FALSE );
    }

    void setInsideProtected()  //JAG-3300 
    {
      this->objType = (((unsigned)(this->objType)) | 0xC0000000);
    }

    VhBoolean getIfFloatingOrPhysicalType() const {
        return ((((objType >> 29) & 0x00000001 ) == 1) ? VH_TRUE : VH_FALSE );
    }

    void setIfFloatingOrPhysicalType()  //JAG-3300 
    {
      this->objType = (((unsigned)(this->objType)) | 0x20000000);
    }
    // PR 13133 : This is not required for this support, but kept for future 
    // use and proper testing.
    VhBoolean getIfItIsAJaguarNode() const {
        return ((((objType >> 30) & 0x00000001 ) == 1) ? VH_TRUE : VH_FALSE );
    }

    /* Added during the support of JAG-3044 */

    inline unsigned getAboluteObjType() const 
    {
        return this->objType ;
    }

    VhBase *getScope() const { return this->scope; }

    void  setScope(VhBase* Scope){
        this->scope=Scope;
    }

    void decompile() ;

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s);
    void *operator new(size_t s,int flag);
    void operator delete(void *p);
#if (defined(__alpha) && !defined(__GNUC__))
    void operator delete(void *p,int flag);
#endif

    void setIsInStatementRegion(VhBoolean );
    void setIsInDeclarativeRegion(VhBoolean );
    void setIsInInterfaceRegion(VhBoolean );
    void setIsInOutsideTheScope(VhBoolean );
    void setIsInBeforeCommaRegion(VhBoolean );
    void setIsInAfterCommaRegion(VhBoolean );
    void setIsInEndBracketRegion(VhBoolean );

    VhBoolean getIsInStatementRegion() const ;
    VhBoolean getIsInDeclarativeRegion() const ;
    VhBoolean getIsInInterfaceRegion() const ;

    //VhBoolean getIsInStatementRegion() const ;
    //VhBoolean getIsInDeclarativeRegion() const ;
    //VhBoolean getIsInInterfaceRegion() const ;
   // VhBoolean getIsInStatementRegion() const ;
    //VhBoolean getIsInDeclarativeRegion() const ;
    //VhBoolean getIsInInterfaceRegion() const ;
    scopeRegionType getCurrentAnalyzingRegion() const ;
};


/*
    This object is created for all name declarations and
    any other object which has a name associated with itself.
    All such objects will have their name inserted into the
    symbol table and have a pointer from the symbol table
    back to this object.
*/
class VhNamed : public VhBase
{
protected :
    char*      name;

public:
    VhNamed(VhBase* ,char* , int , const char* ,VhBoolean = VH_FALSE, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhNamed();
     
    void setName(char* val);
    char* getName() const { return name;}
    VhBoolean  isExtended() const { return (*name == '\\' ? VH_TRUE : VH_FALSE); } 
};


/*
    Derived from VhNamed, and created for all declarations.
    The field usedAsObject is set when the first time
    a particular declaration is used. The field then points
    to an object of type VhObject derived from VhExpr.

    Consequently whenever an expression uses this name/decl.
    we make it point to this VhObject.
*/
class VhDecl: public VhNamed
{
    unsigned _nDeclInfo ;
    VhExpr* usedAsObject;

public:
    VhDecl(VhBase* , char* , int ,
             const char* ,VhBoolean = VH_FALSE, unsigned = 0);
    ~VhDecl() {}

    VhExpr* getObjectUsage() { return usedAsObject; }
    void setObjectUsage(VhExpr* expr) { this->usedAsObject = expr; }

    VhSubTypeInd *getElementSubTypeInd() const ;
    VhSubTypeInd *getExtremeBaseSubTypeInd() const ;
    VhSubTypeInd *getNullEleTypeForRecType() const ;
    VhBoolean getIs2008SpecificType() const ; /* JAG-3134 */
    VhBoolean getIsOfTypeDef(VhObjType ) const ; /* JAG-3173 */
    void getFlatListOfRanges(
        std :: vector<VhExpr *> &, 
        VhBoolean = VH_FALSE, VhBoolean = VH_FALSE) const ;
    VhExpr *getCompleteExprBound(VhExpr *) const ;
    void setAssociated(VhBoolean) ;
    VhBoolean isAssociated() const ;
    void setIsExternFromStdLogicType(VhBoolean bVal);
    VhBoolean getIsExternFromStdLogicType() const;
    void setIsExternFromIeeeType(VhBoolean bVal);
    VhBoolean getIsExternFromIeeeType() const;
};


#endif

