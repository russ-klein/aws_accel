/*==========================================

 This file and related documentation are
 proprietary and confidential to Siemens
         Copyright Siemens 2021

===========================================*/

/************************************************************************

    File             : VhExpr.h

    Author           : Rudra

    Date Started     : 15th May, 1995

    Revision History :

    Classes :
            VhExpr,
                VhOperator,
                    VhUnary,  VhBinary, 
                VhFuncCall,
                VhQExpr,
                VhTypeConv,
                VhOthers, VhAll, VhOpen, VhSelName,
                VhIndName,
                VhSliceName,
                VhPhysicalLit,
                VhIdEnumLit, VhCharLit,
                VhDecLit, VhBaseLit,
                VhBitString, VhString, VhOpString,
                VhNullLit,
                VhAttrbName, VhSignature,
                VhObject,
                VhSimpleName,
                VhAggregate, VhElementAssociation,
                VhAllocator,
                VhSelectedName

    Purpose : Define the expression base class, and the classes for
              the different types of expressions that are derived
              from this expression base class. There are two top level
              expression base classed VhExpr as well as VhAbsExpr.

************************************************************************/
       

#ifndef __VHEXPR__H
#define __VHEXPR__H

class VhDecl;
class VhExpr;
class VhList;
class VhSubTypeInd;
class VhDisRange;
class VhConstraint;

#include "VhConst.h"
#include "VhBase.h"

/**n
    Base class for different types of VHDL expressions derived
    from this class. Members of this base class are :
        exprType : the type of this expresson. Will be a
                   pointer to the corresponding type declaration.
        semanticCheckInfo: stores bits for different semantic checks to be
                           performed during elaboration once the names
                           are resolved.
    This class is used as the base class for expressions like literals etc.
    which are always locally static and need no size information.

                                        Distribution of semanticCheckInfo
                                        ---------------------------------
                                           Width          Offset
used as conditional if elseif expression     1               0
used in sensitivity list of process          1               1
used as psl count                            1               2
the above count should be +ve(1), non-0(0)   1               3
used as top level psl boolean expr           1               4
used as actual for pslParamkind formal       1               5
type of the above pslParamkind               2               6 //00 - const
                                                               //01 - boolean
                                                               //10 - property
                                                               //11 - sequence
used as attribute name                       1               8
flag to distinguish selected names used      1               9 //PR 9422
in use clause as decl item from those
used in use clause over the design-unit
(as context clause).
is an array element (the offset in shadow)   1               10 //PR 10229
is node is a slice of aggregate              1               30
****/

class VhExpr:public VhBase
{
    int      _nSemanticCheckInfo;
    unsigned _nExprInfo ;
    VhDecl*  _pExprType;
    char*    _sFormalName;    //JAG-2948

protected :

    inline unsigned getExprBitFields() const
    {
        return this->_nExprInfo;
    }

    inline unsigned getSementicCheckBitFileds() const
    {
        return this->_nSemanticCheckInfo ;
    }
    
public:
    VhExpr();
    VhExpr(const VhExpr &) ;
    VhExpr(VhBase* , char *, int, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhExpr(){}

    VhDecl* getExprType() const ;
    VhDecl* getAbsoluteExprType() const { return this->_pExprType; }
    void setExprType(VhDecl* );
    // this one is needed for create functions which need not do search and
    // create externals etc while setting type as happens in above function.
    void assignExprType(VhDecl* eType) { this->_pExprType = eType; }

    VhStaticType getStaticType();
    void setStaticType(VhStaticType staticType);

    VhExpr* getStaticValue();
    void setStaticValue(VhExpr* staticValue);

    VhExpr* getExprBound() const ;
    void setExprBound(VhExpr* );
    
    VhExpr* getAbsoluteExprBound();
    
    void setSemanticCheckInfoValue(int dwidth, int offset, int dvalue)
    {
        int num1 = dvalue << offset;
        int num2 = ~(dwidth << offset);
        num2 = (this->_nSemanticCheckInfo & num2);
        this->_nSemanticCheckInfo = num2 | num1;
    }
    int getSemanticCheckInfoValue(int width,int offset)
    {
         int value = width << offset;
         value = this->_nSemanticCheckInfo & value;
         return  (value >> offset);
    }
    int getSemanticCheckInfo()
    {
        return this->_nSemanticCheckInfo;
    }
    void setFromNonDumpPlane()
    {
      this->_nSemanticCheckInfo |= 1 << 12;
    }
    int isFromDumpPlane()
    {
      int isFromNonDump = ((this->_nSemanticCheckInfo & 0x00001000) >> 12);
      return (isFromNonDump)? 0 : 1;
    }
    void setDeleted()
    {
      this->_nSemanticCheckInfo |= 1 << 13;
    }
    int isDeleted()
    {
      return ((this->_nSemanticCheckInfo & 0x00002000) >> 13);
    }
    void setDeletable(int flag)
    {
      if (flag)
         this->_nSemanticCheckInfo |= 1 << 14;
      else
         this->_nSemanticCheckInfo &= ~(1 << 14);
    }
    int isDeletable()
    {
      return ((this->_nSemanticCheckInfo & 0x00004000) >> 14);
    }
    void setInternalEvaluatedExpr()
    {
      this->_nSemanticCheckInfo |= 1 << 15;
    }
    int isInternalEvaluatedExpr()
    {
      return ((this->_nSemanticCheckInfo & 0x00008000) >> 15);
    }
    //PR_14031
    //Use the 16 th bit of the expression to notify, whether
    //the alias designator is string in non object aliasing.
    void setAliasDesigExprStringType()
    {
      this->_nSemanticCheckInfo |= 1 << 16;
    }
    int isAliasDesigExprStringType()
    {
      return ((this->_nSemanticCheckInfo & 0x00010000) >> 16);
    }
    //PR_14463
    //Use the 17th bit of the time expr of a disconnect spec
    //to notify, whether the disconnect spec has "OTHERS" in place
    //of signal list
    void setDisconnectSpecOthersInTimeExpr(int val)
    {
      this->_nSemanticCheckInfo &= ~(0x00020000);
      this->_nSemanticCheckInfo |= val << 17;
    }
    int isDisconnectSpecOthersInTimeExpr()
    {
      return ((this->_nSemanticCheckInfo & 0x00020000) >> 17);
    }
    //PR_14463
    //Use the 18th bit of the time expr of a disconnect spec
    //to notify, whether the disconnect spec has "ALL" in place
    //of signal list
    void setDisconnectSpecAllInTimeExpr(int val)
    {
      this->_nSemanticCheckInfo &= ~(0x00040000);
      this->_nSemanticCheckInfo |= val << 18;
    }
    int isDisconnectSpecAllInTimeExpr()
    {
      return ((this->_nSemanticCheckInfo & 0x00040000) >> 18);
    }
    //PR_15039
    //Use the 19th bit of the opstring to notify, whether 
    //the FuncCall has full path i.e. STD.STANDARD in it
    //while it is called, so that it can be checked while
    //decompilation
    void setOpstringImplicitFuncCallHasFullPath()
    {
      this->_nSemanticCheckInfo |= 1 << 19;
    }
    int isOpstringImplicitFuncCallHasFullPath()
    {
      return ((this->_nSemanticCheckInfo & 0x00080000) >> 19);
    }
    //PR_15091
    //Under the PI vhHasAnySizeMisMatch(), we used to store this
    //warning occuraance info (warnid-170,215,531) in the expression.
    //The below mentioned PI sets the bit whenever the mentioned
    //warning occurred.
    void setKeepSizeMisMatchInfoInExpr(int value)
    {
      this->_nSemanticCheckInfo |= value << 20;
    }
    //PR_15091
    //The below mentioned PI used to get the warning info from the expr
    //which occurred during analysis.
    int isKeepSizeMisMatchInfoInExpr()
    {
      return ((this->_nSemanticCheckInfo & 0x00300000) >> 20);
    }

    // Reusing 20 and 21 with an assumption that, previous information
    // is not required in Mentor flow. These bits were used under
    // API, vhKeepSizeMisMatchInfoInExpr(). Changes are also made in APIs
    // so that this API can't be enabled under vhMentorCompatible() 
    // (Bit 20 is used now, 21 is free under MentorCompatible)
    void setFuncallArgCreatedFromDefaultVal()
    {
      this->_nSemanticCheckInfo |= 1 << 20;
    }
    int isFuncallArgCreatedFromDefaultVal()
    {
      return ((this->_nSemanticCheckInfo & 0x00100000) >> 20);
    }
    
    // PR_15195
    //Use the 22th bit of the expr to notify, whether its type was 
    //visible before setting of exprtype on it.
    void setNotVisibleType()
    {
      this->_nSemanticCheckInfo |= 1 << 22;
    }
    int isNotVisibleType()
    {
      return ((this->_nSemanticCheckInfo & 0x00400000) >> 22);
    }
    //JAG-2798. The 23rd bit used to know whether an expression
    //is of inertial type or not. (VHDL-2008 feature)
    void setInertial()
    {
      this->_nSemanticCheckInfo |= 1 << 23;
    }
    int isInertial()
    {
      return ((this->_nSemanticCheckInfo & 0x00800000) >> 23);
    }

    // Bit 23 is unused in trunk, as it used for 2008 purpose 

    //JAG_2847
    //Use the 24 th bit of the expression to notify, whether
    //the alias decl for this aliased expr has no subtype 
    //indication present and the subtype indication was 
    //created during analysis.
    void setAliasDeclOfThisAliasExprHasNoSubTypeInd()
    {
      this->_nSemanticCheckInfo |= 1 << 24;
    }
    int isAliasDeclOfThisAliasExprHasNoSubTypeInd()
    {
      return ((this->_nSemanticCheckInfo & 0x01000000) >> 24);
    }
    
    void setIsExprWrittenInBracket()
    {
        this->_nSemanticCheckInfo |= 1 << 25 ;
    }

    int getIsExprWrittenInBracket() const
    {
        return ((this->_nSemanticCheckInfo & 0x02000000) >> 25);
    }

    //JAG-3487. To mark the shadowFits expression validation
    //context
    void setWOContextSetExprTypeCallReq()
    {
      this->_nSemanticCheckInfo |= 1 << 25;
    }
    int isWOContextSetExprTypeCallReq()
    {
      return ((this->_nSemanticCheckInfo & 0x02000000) >> 25);
    }
    void markPossibleOpSearchDone()
    {
      this->_nSemanticCheckInfo |= 1 << 26;
    }
    void unmarkPossibleOpSearchDone()
    {
      this->_nSemanticCheckInfo &= 0 << 26;
    }
    int isPossibleOpSearched()
    {
      return ((this->_nSemanticCheckInfo & 0x04000000) >> 26);
    }

    // Info related to concat operand, required during
    // new evaluation flow.
    void setLhsConcatOpElemType()
    {
      this->_nSemanticCheckInfo |= 1 << 27;
    }
    int isLhsConcatOpElemType()
    {
      return ((this->_nSemanticCheckInfo & 0x08000000) >> 27);
    }
    void setRhsConcatOpElemType()
    {
      this->_nSemanticCheckInfo |= 1 << 28;
    }
    int isRhsConcatOpElemType()
    {
      return ((this->_nSemanticCheckInfo & 0x10000000) >> 28);
    }
    void setInitValOfConstant()
    {
      this->_nSemanticCheckInfo |= 1 << 29;
    }
    int isInitValOfConstant()
    {
      return ((this->_nSemanticCheckInfo & 0x20000000) >> 29);
    }
    void setAggregateSliceNode()
    {
      this->_nSemanticCheckInfo |= 1 << 30;
    }
    int isAggregateSliceNode()
    {
      return ((this->_nSemanticCheckInfo & 0x40000000) >> 30);
    }

    void setFormalStr(char *name) { this->_sFormalName = name; }
    char* getFormalStr() const { return this->_sFormalName; }

    /* Some utility functions */

    VhSubTypeInd *getExprSubTypeIndIfValid() const ;
    VhBoolean getIsObtainedFrom2008SpecificType() const ;
};


/****
    Base class for VHDL expressions which need to preserve static type,
    expression size and static value information.
    Members of this base class are :
        _eStaticType : defines the static type of the expression.
            Can be :
                locally static (i.e can be evaluated at compile time)
                globally static (to be evaluated after elaboration)
                not static (can be evaluated only at run-time)
        _pStaticValue : if locally static, contains the static value
            of this expression. Pointer to another expression, which
            be a string, literal, etc.
        _pExprSize : this field stores the possible expression bounds
            of this expression
****/

class VhAbsExpr:public VhExpr
{
    VhStaticType _eStaticType;
    VhExpr*      _pStaticValue;
    VhExpr*      _pExprSize;
    
public:
    VhAbsExpr();
    /* Added during the support of JAG-3044 */
    VhAbsExpr(const VhAbsExpr &A_Bs);

    VhAbsExpr(VhBase* , char *, int, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhAbsExpr(){}

    VhStaticType getStaticType() const { return this->_eStaticType; }
    void setStaticType(VhStaticType StaticType) { this->_eStaticType = StaticType; }

    VhExpr* getStaticValue() const { return this->_pStaticValue; }
    void setStaticValue(VhExpr* StaticValue) { this->_pStaticValue = StaticValue; }

    VhExpr* getExprBound() const ;
    void setExprBound(VhExpr* ExprSize){ this->_pExprSize = ExprSize; }

    VhExpr* getAbsoluteExprBound() const { return this->_pExprSize; }
};


/****
    Base class for unary and binary expressions. This class
    stores the operator used in the expression. Members are :
        _eOpType : the operator type
        _eOpCategory : the category to which this operator belongs i.e
            (relational, logical, adding, etc.)
        _pOperatorMaster : in case of an overloaded operator, points
            to the declaration of this overloaded operator
            function
****/

class VhOperator:public VhAbsExpr
{
    VhOpCategory   _eOpCategory;
    VhOpType       _eOpType;
    VhDecl*        _pOperatorMaster;

public:
    VhOperator(VhBase* , char *, int , VhOpType ,
               VhOpCategory, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhOperator(){}

    void setOpType(VhOpType val){this->_eOpType = val;}
    VhOpType getOpType() const{ return this->_eOpType; }
    void setOpCategory(VhOpCategory val){this->_eOpCategory = val;}
    VhOpCategory getOpCategory(){ return this->_eOpCategory; }

    VhDecl* getMaster() const { return this->_pOperatorMaster; }
    void  setMaster(VhDecl* Master){ this->_pOperatorMaster = Master; }
    
    void decompile() {}
};


/****
    Class for unary expressions. The operator name, type, etc.
    are stored in the base class VhOperator. This class just
    stores the right hand side of the unary expression, which
    is another expression.
****/

class VhUnary:public VhOperator
{
    VhExpr *_pExprOfUnary;

public: 
    VhUnary(VhBase* , char *, int , VhOpType , VhOpCategory ,
            VhExpr*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhUnary(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    VhExpr* getExpr() const{ return this->_pExprOfUnary; }
    void    setExpr(VhExpr* e) { this->_pExprOfUnary = e; }

    
    void decompile();
};


/****
    Class for binary expressions. The operator name, type, etc.
    are stored in the base class VhOperator. This class just
    stores the right and left hand sides of the binary expression,
    which will also be other expressions.
****/

class VhBinary:public VhOperator
{
    VhExpr *_pLeftExpr ;
    VhExpr *_pRightExpr ;

public: 
    VhBinary(VhBase* , char *, int , VhOpType ,
             VhOpCategory , VhExpr* , VhExpr* , unsigned = 0); 
    ~VhBinary(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    VhExpr* getLhsExpr() const{ return this->_pLeftExpr; }
    VhExpr* getRhsExpr() const{ return this->_pRightExpr; }

    void setLhsExpr(VhExpr* e) { this->_pLeftExpr = e; }
    void setRhsExpr(VhExpr* e) { this->_pRightExpr = e; }

    void decompile();
};


/****
    Class for function call expression. Members are :
        _pFuncName : the name using which the function has been
            invoked (could be simple name, selected name, opstring etc.)
        _pFuncCallMaster : pointer to the master function declaration,
            which has been invoked
        _pAssociationList : the list of arguments passed to this
            function. This list will contains objects of type
            VhAssociation in case of named association else it will
            contain a simple list of expressions in positional order.
****/

class VhFuncCall:public VhAbsExpr
{
    VhExpr*          _pFuncName;
    VhDecl*          _pFuncCallMaster;
    VhList*          _pAssociationList;  

public:
    VhFuncCall(VhBase* , char *, int , VhExpr* , VhDecl* ,
               VhList* , unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhFuncCall(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    
    inline VhExpr* getFuncName() const { return this->_pFuncName; }
    inline VhList* getAssociationList() const { return this->_pAssociationList; }
    void setAssociationList(VhList *);
 
    VhDecl* getMaster() const ;
 
    inline VhDecl*  getAbsoluteMaster() const { return this->_pFuncCallMaster; }
    inline void setMaster(VhDecl* decl) { this->_pFuncCallMaster = decl; }
    inline void setFuncName(VhExpr* expr){ this->_pFuncName=expr; }
  
    // added for PR 7735 : definition in file addUserAttribute.cxx
    void setNamedAssocInfo(void *info);
    void *getNamedAssocInfo();

    void decompile();
};


/****
    Class for VHDL qualified expressions. Has 2 members :
        type : the type which is being used for qualification
        expr : the expression being qualified
****/

class VhQExpr:public VhAbsExpr
{
    VhDecl*  type;
    VhExpr*  expr;

public:
    VhQExpr(VhBase* , char *, int , VhDecl* , VhExpr*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhQExpr(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setType(VhDecl*  val){type = val;}
    VhDecl* getType() const { return type; }
    void  setExpr(VhExpr*  val){expr = val;}
    VhExpr* getExpr() const { return expr; }
    void  setPrefix(VhExpr*  val){/*MISSING SET*/}
    VhExpr* getPrefix();

    void decompile();
};


/****
    Class for VHDL type conversion expressions. Has 2 members :
        type : the type which is being used for type conversion
        expr : the expression whose type is being converted
****/

class VhTypeConv:public VhAbsExpr
{
    VhDecl*  type;
    VhExpr*  expr;

public:
    VhTypeConv(VhBase* , char *, int , VhDecl* , VhExpr*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhTypeConv(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setType(VhDecl*  val){type = val;}
    VhDecl* getType() const { return type; }
    void  setExpr(VhExpr*  val){expr = val;}
    VhExpr* getExpr() const { return expr; }
    void  setTypeMark(VhExpr*  val){/*MISSING SET*/}
    VhExpr* getTypeMark();

    void decompile();
};


/****
    Class corresponding to the keyword OTHERS which may
    cocur in aggregates, association lists, etc.
****/

class VhOthers:public VhExpr
{

public:
    VhOthers(VhBase* , char *, int, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhOthers(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void decompile();
};


/****
    Class corresponding to the keyword ALL which may
    cocur in use clauses, selected names, etc.
****/

class VhAll:public VhExpr
{
    
public:
    VhAll(VhBase* , char *, int, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhAll(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void decompile();
};


/****
    Class corresponding to the keyword OPEN which may
    cocur in association list actuals, etc.
****/

class VhOpen:public VhExpr
{
    
public:
    VhOpen(VhBase* , char *, int, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhOpen(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void decompile();
};


/****
    Internediate Class corresponding to a selected name. Objects of
    this class are populated directly in the parser using the prefix
    and suffix of the selected name, both of which will be expressions.
    These are later translated to proper expressions of type
    VhSelectedName.
****/

class VhSelName:public VhExpr
{
    VhExpr*     prefix;
    VhExpr*     suffix;

public:
    VhSelName(VhBase* , char *, int , VhExpr*  , VhExpr*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhSelName(){
    }

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s);
    void *operator new(size_t s,int flag);
    void operator delete(void *p);
    VhExpr* getPrefix() const { return prefix; }
    VhExpr* getSuffix() const { return suffix; }
    
    void decompile();
};


/****
    Class for indexed names. Members are :
        prefix : the prefix expression being indexed
        noOfDim : the number of array indices used in the
            indexed name
        exprList : the list of indices used in this indexed
            name
****/

class VhIndName:public VhAbsExpr
{
    VhExpr*   prefix;
    int       noOfDim;
    VhList*   exprList;

public:
    VhIndName(VhBase* , char *, int , int , VhExpr* ,
              VhList*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhIndName() { }

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    void operator delete(void *p);

    void  setNoOfDim(int  val){noOfDim = val;}
    int getNoOfDim() const { return noOfDim; }
    void  setPrefix(VhExpr*  val){prefix = val;}
    VhExpr* getPrefix() const { return prefix; }
    void  setExprList(VhList*  val){exprList = val;}
    VhList*  getExprList() const { return exprList; }

    void decompile();
}; 


/****
    Class corresponding to VHDL slice names. Members are :
        prefix : the expression whose slice is being taken
        disRange : this field defines the slice or the range
            used in the slice name.
****/

class VhSliceName:public VhAbsExpr
{
    VhExpr*     prefix;
    VhDisRange* disRange;

public:
    VhSliceName(VhBase* , char *, int , VhDisRange* , VhExpr*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhSliceName(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setPrefix(VhExpr*  val){prefix = val;}
    VhExpr* getPrefix() const { return prefix; }
    void  setDisRange(VhDisRange*  val){disRange = val;}
    VhDisRange*  getDisRange() const { return disRange; }
    
    void decompile();
};


/****
    Class for physical literals when they occur in an expression.
    The members are :
        val : the literal value, most possibly a decimal literal
        unitDecl : the unit used in the expression. Points to the
            parent physical type definition for this unit name.
****/

class VhPhysicalLit:public VhAbsExpr
{
    VhExpr *val;
    VhDecl *unitDecl;

public:
    VhPhysicalLit(VhBase* , char *, int , VhExpr *, VhDecl *, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhPhysicalLit() {}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setUnitDecl(VhDecl*  val){unitDecl = val;}
    VhDecl* getUnitDecl() { return unitDecl; }
    void  setUnitName(char*  val){unitDecl->setName(val);}
    char* getUnitName();
    void  setVal(VhExpr *  val){ this->val = val;}
    VhExpr *getVal(){ return val; }

    void decompile();
};


/****
    Class created for identifier enumeration literals when
    used in expressions. Members are :
        name : the name of the literal
        value : the integer value of this literal as determined
            from its numerical order in the definition of this
            literal.
****/

class VhIdEnumLit:public VhExpr
{
    char*       name;
    int         value;

public:
    VhIdEnumLit(VhBase* , char *, int , char*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    VhIdEnumLit(VhBase* , char *, int , VhIdEnumLit*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhIdEnumLit();

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    
    void setName(char *val);
    char *getName() const{ return name; }

    int getValue() const { return value; }
    void setValue(int Value){ this->value=Value; }
    
    void decompile();
};


/****
    Class created for character enumeration literals when
    used in expressions. Members are :
        ch : the literal character
        value : the integer value of this literal as determined
            from its numerical order in the definition of this
            literal.
****/

class VhCharLit:public VhExpr
{
    char    ch;
    int     value;

public:
    VhCharLit(VhBase* , char *, int , char, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    VhCharLit(VhBase* , char *, int , VhCharLit*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhCharLit(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setChar(char  val){ch = val;}
    char getChar() const{ return ch; }

    int getValue(){ return value; }
    void setValue(int Value){ this->value=Value; }

    void decompile();
};


/****
    Class created for VHDL decimal literals. Members are :
        integer_part : the string corresponding to the integer
            part of this deimal literal
        floating_part : the string corresponding to the flaoting
            part of this deimal literal
        exponent : the string corresponding to the exponent
            part of this deimal literal
        value : stores the actual value of this decimal literal.
            FLOATTYPE is reduced to float/double/long at compile time.
            (for the sake of portability)
****/

class VhDecLit:public VhExpr
{
    char*     integer_part;
    char*     floating_part;
    char*     exponent;
    int       value;

public:
    VhDecLit(VhBase* , char *, int , char* , char* , 
             char*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    VhDecLit(VhBase* , char *, int , VhDecLit*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhDecLit();

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    
    char* getInteger() const { return integer_part; }
    void  setFloat(char*  val);
    char* getFloat() const { return floating_part; }
    void  setExponent(char*  val);
    char* getExponent() const { return exponent; }

    void  setValue(FLOATTYPE   val){/* = val;MISSING SET*/}
    FLOATTYPE getValue(); // defined in VhExpr.cxx
    // PR 10987: added members setValue() and setInteger().
    void setValue(int Value){ this->value=Value; }
    void setInteger(char *ip);

    void decompile();
};


/****
    Class created for VHDL based literals. Members are :
        base : the integral base used in this based literal
        value : stores the entire base literal apart from the
            base
        isFloating : boolean variable which indicates if this
            base literal has a floating part.
****/

class VhBaseLit:public VhExpr
{
    unsigned _nBaseLitInfo ;
    char*     value;

public:
    VhBaseLit(VhBase* , char *, int , int ,  char*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhBaseLit();

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setValue(char*  val);
    char* getValue() const { return value; } 
    void  setIsValueHasHash(VhBoolean  val){/*MISSING SET*/}
    VhBoolean getIsValueHasHash();
    int getBaseType() const ;
    void setBaseType(unsigned ) ;
    
    VhBoolean getIsFloating() const ;
    void setIsFloatType(VhBoolean ) ;

    void decompile();
};


/****
    Class for VHDL bit string literals. Members are :
        base : the base type used with this bit string literal.
            (can be either binary, octal or hex)
        value : the actual bit string with the base removed
****/

class VhBitString:public VhAbsExpr
{
    VhBaseType   base;
    char*        value;
    int          len;  //"len" should be provided as -1 if len field is not provided explicitly.

public:
    VhBitString(VhBase* , char *, int , VhBaseType , char*, int, unsigned = 0);
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhBitString();

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setBaseType(VhBaseType  val){base = val;}
    VhBaseType getBaseType() const { return base; }
    void  setValue(char*  val);
    char* getValue() const { return value; } 
    void  setLen(int  length){len = length;}
    int getLen() const { return len; } 

    void decompile();
};


/****
    Class for VHDL string literals. Has just one member which
    stores the actual string used in this string literal.
****/

class VhString:public VhAbsExpr
{
    char*    string;

public:
    VhString(VhBase* , char *, int , char*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhString();

    void  setString(char*  val);
    char* getString() const { return string; }

    void decompile();
};


/****
    Class for operator strings derived from VhString. This
    class is used when we have an operator function declaration
    or call. Members are :
        opType : the operator type used in the string
        opCat : the operator category (relational, adding, etc.)
****/

class VhOpString:public VhString
{
    VhOpType       opType;
    VhOpCategory   opCat;

public:
    VhOpString(VhBase* , char *, int , char* ,VhOpType ,
                VhOpCategory, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhOpString(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setOpType(VhOpType  val){opType = val;}
    VhOpType getOpType() const{ return opType; }
    void  setOpCategory(VhOpCategory  val){opCat = val;}
    VhOpCategory getOpCategory(){ return opCat; }

    void decompile();
};


/****
    Class for VHDL NULL literal expressions.
****/

class VhNullLit:public VhExpr
{

public:
    VhNullLit(VhBase* , char *, int, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhNullLit(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    
    void decompile();
};


/****
    This class is defined for VHDL signatures that can be
    registered on overloaded subprograms or literals. Members are :
        listOfType : the list of argument types of the subprogram
        returnType : the return type if any
****/

class VhSignature:public VhBase
{
    VhList*       listOfType;
    VhDecl*       returnType;

public:
    VhSignature(VhBase* , char *, int , VhList* = 0,
                VhDecl* = 0, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhSignature() {}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setTypeList(VhList*  val){listOfType = val;}
    VhList* getTypeList() const { return listOfType; }
    VhList* getTypeMarkList() ; 
    void  setReturnType(VhDecl*  val){returnType = val;}
    VhDecl* getReturnType() const { return returnType; }
    
    VhExpr* getReturnTypeMark();

    void decompile();
};


/****
    Class for VHDL predefined and user defined attribute names.
    Members are :
        attrbType : if predefined then the attribute type, else
            user_defined
        signature : the signature if any associated with this
            attribute name
        attribute : In case of user defined attributes, this points
            to the attribute declaration.
        prefix : the prefix expression on which the specified attribute
            is acting
        expr : the optional suffix which may be passed as an argument
            to the attribute
****/

class VhAttrbName:public VhAbsExpr 
{
    VhAttrbType       attrbType;
    VhSignature*      signature;
    VhDecl*           attribute;
    VhExpr*           prefix;
    VhExpr*           expr; 

public:
    VhAttrbName(VhBase* , char *, int , VhAttrbType , 
                VhExpr* , VhDecl* , VhSignature* =0,
                VhExpr* =0, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhAttrbName(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setAttrbType(VhAttrbType  val){attrbType = val;}
    VhAttrbType getAttrbType() const { return attrbType; }
    VhExpr* getPrefixExpr() const ;
    VhExpr* getOrigPrefixExpr() const { return this->prefix; }
    void    setPrefixExpr(VhExpr* Prefix) { this->prefix = Prefix; }
    void  setExpr(VhExpr*  val){expr = val;}
    VhExpr* getExpr() const { return expr; }
    void  setAttribute(VhDecl*  val){attribute = val;}
    VhDecl* getAttribute() const{ return attribute; }
    
    char* getAttributeName();

    VhSignature* getSignature() const { return signature; }
    void setSignature(VhSignature*  Signature) { this->signature=Signature; }

    void  setIsSignalTypeAttribute(VhBoolean  val){/*MISSING SET*/}
    VhBoolean isSignalTypeAttribute();
    void  setIsValueTypeAttribute(VhBoolean  val){/*MISSING SET*/}
    VhBoolean isValueTypeAttribute();
    void  setIsFuncTypeAttribute(VhBoolean  val){/*MISSING SET*/}
    VhBoolean isFuncTypeAttribute();
    VhBoolean isUserDefAttribute();

    VhExpr *getImageExprBound() const ;
    VhConstraint *getCompleteConstraint() const ; /* JAG-3422 */

    void decompile();

};


/****
    Class for a simple expression which uses a simple name
    having a declaration somewhere. In order to use this name
    in an expression tree, this class is used which creates
    a simple expression object. The field of this class, actualObj
    points to the actual declaration for this name.
****/

class VhObject:public VhAbsExpr
{
    VhDecl*    actualObj;

public:
    VhObject(VhBase* , char *, int , VhDecl*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhObject() { }

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    void operator delete(void *p);

    VhDecl* getActualObj() const { return this->actualObj; }
    void    setActualObj(VhDecl *decl) { actualObj = decl; }

    char* getActualObjName() ;
    
    void decompile();
};

#ifndef _preprocess_for_cgen
typedef struct _ExtNameStaticInfo
{
    VhExpr* staticValue;
    void*   typeInfo;     
    _ExtNameStaticInfo()
    {
        staticValue = NULL;
        typeInfo    = NULL;
    }
} ExtNameStaticInfo;
#endif

/****
    Class for VHDL external names.
    Members are :
        pathNameList : This list store the path name element of the external name.
                       Last element of this list is the simple name object of the
                       external name.
        subTypeInd   : Subtype Indication of the external name
        actualObj    : The actual decl of the external name object. During analysis
                       it set to null; As per LRM-2008 we should assume the existence
                       of pathname element during analysis.
        info         : This field stores various info about the external name.
                       bit[31..30] => constant/signal/variable/
                       bit[29..28] => Package/Absolute/Relative
                       bit[27]     => isParentInVerilog
                       bit[26]     => isInsideGenHeader
                       bit[25]     => isElabOnly
                       bit[24]     => isActualCopiedNode
                       bit[23..16] => reserved for future use
                       bit[15..0]  => Count of '^'s
        objectList   : This will hold the list of objects corresponding to every
                       element in the path list. This will be stored as void *
****/
class VhExternalName:public VhAbsExpr
{
    VhList*       pathNameList;
    VhSubTypeInd* subTypeInd;
    VhDecl*       actualObj;
    unsigned      info;
    VhList*       objectList;
    void*         staticInfo;
    char*         lastEntityName;
    char*         parentNetName;

public:
    VhExternalName(VhBase *Scope, char *fname, unsigned startLineNo, unsigned endLineNo, VhList* PathNameList, 
                        VhSubTypeInd* SubTypeInd, VhObjType ClassType, int PathCategory, unsigned NoOfUpRef);
    ~VhExternalName() {}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s);
    void *operator new(size_t s,int flag);

    char*                   getName() const;
    void                    setName(char*  val);
    VhExternalPathName      getPathNameCategory() const;
    void                    setPathNameCategory(VhExternalPathName category);
    VhDecl*                 getActualObj() const { return this->actualObj; }
    void                    setActualObj(VhDecl *decl);
    void*                   getTypeInfo() const;
    void                    setTypeInfo(void *pTypeInfo);
    VhExpr*                 getStaticValue() const;
    void                    setStaticValue(VhExpr *val);
    VhList*                 getPathNameList() const { return this->pathNameList; }
    void                    setPathNameList(VhList*  val) { pathNameList = val; }
    VhList*                 getObjectList() const { return this->objectList; }
    void                    addNodeToObjectList(VhBase *node);
    VhSubTypeInd*           getSubTypeInd() const { return this->subTypeInd; }
    void                    setSubTypeInd(VhSubTypeInd*  val) { subTypeInd = val; }
    VhBoolean               isPackPathName() const;
    VhBoolean               isAbsPathName() const;
    VhBoolean               isRelPathName() const;
    VhObjType               getClassType() const;
    void                    setClassType(VhObjType classType);
    int                     getNoOfUpRef() const;
    void                    setNoOfUpRef(unsigned val);
    VhBoolean               isInsideGenHeader() const;
    void                    setInsideGenHeader(VhBoolean val);
    VhBoolean               isMixed() const;
    void                    setIsMixed(VhBoolean val);
    VhBoolean               isElabOnly() const;
    void                    setIsElabOnly(VhBoolean val);
    VhBoolean               isActualCopied() const;
    void                    setActualCopied(VhBoolean val);
    void                    setLastEntityName(char *name);
    void                    setParentNetName(char *name);
    char*                   getLastEntityName() { return lastEntityName; }
    char*                   getParentNetName() { return parentNetName; }

    void decompile();
};

/****
    This class is introduced for the simple expression inside PSL VUnit
    which uses a simple name, the declaration of which couldn't be resolved
    during analysis, and hence cannot be translated to a VhObject.
****/

class VhShadowObject:public VhAbsExpr
{
    char*    name;

public:
    VhShadowObject(VhBase* , char *, int , char*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */

    ~VhShadowObject();

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    void operator delete(void *p);

    void  setName(char*  val);
    char* getName() const { return name; }

    void decompile();
};

/****
    Not all names can be translated to a VhObject. Hence this
    class is used for simple names that occur in an expression.
    The members are :
        name : the name used in the expression
        nameType : the type of this name. Can be any one of
            the enumerated types for VhNameType.
****/

class VhSimpleName:public VhExpr
{
    char*      name;
    VhNameType nameType;

public:
    VhSimpleName(VhBase* , char *, int , char* , VhNameType, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhSimpleName() ;

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;
    void operator delete(void *p);

    void  setName(char*  val);
    char* getName() const{ return name; }

    VhNameType getNameType() const{ return nameType; }
    void setNameType(VhNameType NameType){ this->nameType=NameType; }

    void decompile();
};


/****
    Class for VHDL aggregate expressions. The member is :
        listOfElementAssociation : this expression maintains a
            list of element associations used for defining the
            aggregate. The objects of this list will of the type
            VhElementAss.
        original : bit 0 - preserved the information whether it is original(1) 
                           or created from association(0).
                   bit 1 - set to 1 if it has any 'open' element.
****/

class VhAggregate:public VhAbsExpr
{
    VhList*      listOfElementAssociation;
    unsigned     original; 
    
public:
    VhAggregate(VhBase* , char *, int , VhList*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    VhAggregate(VhList* listOfElementAssociation);
    ~VhAggregate(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setListOfElementAss(VhList*  val){listOfElementAssociation = val;}
    VhList* getListOfElementAss(){ return listOfElementAssociation; }

    inline void setIsConsiderRangeFromSource(VhBoolean A_Flag) 
    {
        this->original = (this->original & ~0x00000004) | (A_Flag << 2);
    }
 
    inline VhBoolean getIsConsiderRangeFromSource()
    {
        return (VhBoolean)((this->original >> 2) & 0x00000001);
    }
    
    VhBoolean hasOpenSubElement() 
               { return (VhBoolean)((this->original >> 1) & 0x01); }
    void setOpenSubElementInfo(VhBoolean flag) 
               { this->original = (this->original & ~0x02) | (flag << 1); } 
    VhBoolean    isOriginal() 
               { return (VhBoolean)(this->original & 0x01); }
    unsigned     getOriginal()
               { return this->original; }
    void         setOriginal(unsigned flag)
               { this->original = flag; }
    
    void decompile();
};


/****
    Class for defining the basic element association objects used
    in an aggregate expression. The formal part (or the lhs part of
    the association) contains a list of choices each of which will
    be a derived expression. The actual part contains the expression
    to be assigned to the choices.
****/

class VhElementAss:public VhExpr
{
    VhList*        choices;
    VhExpr*        expr;

public:
    VhElementAss(VhBase* , char *, int , VhExpr* ,
                    VhList*  = NULL, VhBoolean = VH_FALSE, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhElementAss() {}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setChoices(VhList*  val){choices = val;}
    VhList* getChoices() { return choices; }
    VhExpr* getRhsExpr() { return expr; }
    void    setRhsExpr(VhExpr *rhsExpr) { this->expr = rhsExpr; }

    void decompile();
};


/****
    Class for VHDL Allocator expressions. This object
    is created when we have the new operation on some
    access type. This access type is the operand of the
    allocator expression, which is stored as a member in
    this class.
****/

class VhAllocator:public VhExpr
{
    VhExpr *allocOpnd;

public:
    VhAllocator(VhBase* , char *, int , VhExpr*, unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhAllocator() {}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    void  setExpr(VhExpr *  val){ this->allocOpnd = val;}
    VhExpr *getExpr() { return this->allocOpnd; }
    void decompile();
};


/****
    Class for VHDL selected names. Members are :
        selList : the list of expressions used for creating this
            selected name
        actualObject : the actual object which this selected name
            refers to.
****/

class VhSelectedName:public VhAbsExpr
{
    VhList*   selList;
    VhDecl*   actualObject;
    
public:
    VhSelectedName(VhBase* ,char* ,int , unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhSelectedName(){}

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    VhList*  getSelList(){ return selList; }
    void  setSelList(VhList* namelist) { this->selList = namelist; }

    VhDecl* getActualObject(){ return actualObject; }
    void setActualObject(VhDecl* ActualObject){ this->actualObject=ActualObject; }

    void decompile();
};

class VhShadowFITS:public VhExpr {
    char* name;
    VhList* assList;
    VhExpr* expr;
    int offset;
    int position;
    VhBase *parent;

public:
    VhShadowFITS (VhBase* , char* , int , char*  ,
             VhList* , VhExpr* , unsigned = 0); 
    /* "unsigned = 0" Added during the support of JAG-2930. */
    ~VhShadowFITS () ;

    // overloaded new/delete operators to invoke memory manager
    void *operator new(size_t s) ;
    void *operator new(size_t s,int flag) ;

    char* getName() {
        return this->name;
    }
    void  setExpr(VhExpr*  val){expr = val;}
    VhExpr* getExpr() {
        return this->expr;
    }
    VhList* getList() {
        return this->assList;
    }
    void setParent(VhBase *Parent) {
        this->parent = Parent;
    }
    void setOffset(int Offset) {
        this->offset = Offset;
    }
    VhBase *getParent() {
        return this->parent;
    }
    int getOffset() {
        return this->offset;
    }
    void setPosition(int Position) {
        this->position = Position;
    }
    int getPosition() {
        return this->position;
    }
};

#endif    
