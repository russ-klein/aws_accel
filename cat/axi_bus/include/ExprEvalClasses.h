/*==========================================

 This file and related documentation are
 proprietary and confidential to Siemens
         Copyright Siemens 2021

===========================================*/

#ifndef __EXPR_EVAL_CLASSES_H__
#define __EXPR_EVAL_CLASSES_H__

#include <map>
#include <stack>
#include <string>
#include <iostream>
#include <limits>
#include <cmath>
#include <cerrno>
#include <exception>
#if (_MSC_VER == 1800)
#include <algorithm>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "VhExpr.h"

//#define  __JAG_EVAL_MEM_CHECK__ 1

//#define EXCEPT_SIGN_BIT_ALL_ONE_64_BIT 0x7FFFFFFFFFFFFFFFLL
#define ZERO_64_BIT                    0LL
#define ONE_64_BIT                     1LL
#define OPEN_DATA_64_BIT               0x0000004F     // Ascii 'O'

#define PTR_SIZE 8

#if (defined(WIN32) || defined(WIN64))
    #ifdef __GNUC__ // For Cygwin Gnu Gcc
        typedef          long long int64T;
        typedef unsigned long long uint64T;
    #else
        typedef          __int64   int64T;
        typedef unsigned __int64   uint64T;
    #endif
    #ifdef max
        #undef max
    #endif
    #ifdef min
        #undef min
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#else
    typedef          long long int64T;
    typedef unsigned long long uint64T;
#endif

typedef unsigned char   uint8T;
typedef          int    int32T;
typedef unsigned int    uint32T;
typedef          float  real32T;
typedef          double real64T;

#if (defined(WIN32) || defined(WIN64))
    #ifndef __GNUC__ // For Windows VC

#if (_MSC_VER != 1800)
        inline int64T abs(int64T val)
        {
            return ::_abs64(val);
        }
        #define strtoll _strtoi64 
#endif
/*
        inline real64T pow(real64T base, int64T pwr)
        {
            return ::pow((real64T)base, (real64T)pwr);
        }
        inline int64T strtoll( const char *nptr, char **endptr, int base )
        {
            return ::strtoll(nptr,endptr,base);
        }
*/
    #endif
#endif

class VhEEAddress;
class VhValueEEAddress;
class VhAllocEEAddress;
class TypeInfo; 

extern VhEEAddress* vha_deleteEEAddress(VhEEAddress *addr);
extern void         vha_deleteTypeInfo(TypeInfo *&pTypeInfo);
extern void         vha_errorAt(char *fname, unsInt A_NodeLineNo, int msgId,...);
extern void         vha_error(const char *,...);
extern int          vh_strcasecmp(const char*, const char*);
extern char*        vhw_strdup(const char *);

extern VhDecl *vhGi_stringSTD ;
extern char   *vhGi_currentDataFileName;

#ifdef __JAG_EVAL_MEM_CHECK__
extern std::map<VhEEAddress*, VhEEAddress*> eeAddrMapG;
extern std::map<TypeInfo*, TypeInfo*>       eeTypeMapG;
#endif

extern uint8T maskFromL1[8];
extern uint8T maskFromR1[8];
extern uint8T maskFromL0[8];
extern uint8T maskFromR0[8];
extern std::string vha_getEnvVarExpandedPathName(const std::string & sPathName);

static const char StdLogicCharFromInt[9] = {'U','X','0','1','Z','W','L','H','-'};
//static const char StdLogicX01ZFromInt[9] = {'X','X','0','1','Z','X','0','1','X'};

enum EEerrCode
{
    EE_SUCCESS = 0,
    EE_FAILURE,
    EE_NULL_RANGE,
    EE_NOT_A_RANGE,
    EE_RETURN_WITHOUT_VAL,
    EE_COPY_SIZE_MISMATCH,
    EE_COPY_UNCONSTRAINED,
    EE_COPY_LHS_NULL,
    EE_DONT_PROCEED_TO_COPY,
    EE_PROCEED_TO_PARTIAL_COPY,
    EE_MALLOC_FAILURE
};

enum EEDataType
{
    staticData    = 1,
    nonStaticData = 2,
    openData      = 4
};

enum EEAddrExprType 
{
    ErrorType = 0,
    Record,
    Array,
    Scalar,
    AccessType
};

enum ExprConstraintsType
{
    constraintArrayType = 0,
    unConstraintArrayType,
    intRangeConstraintType,
    realRangeConstraintType,
    errorConstraintType
};

struct VhFileOpenException : std::exception 
{
      virtual const char* what() const throw() {return "VhFileOpenException!\n";}
};

// class EvalEnvelop is the data structure passed in function vha_evaluate_new
// to pass different informations together
class EvalEnvelop
{
private:
    TypeInfo           *m_typeInfo;
    VhAllocEEAddress   *m_allocAddr;
    bool                m_islhs;
    EEerrCode           m_status;
    bool                m_isEvalLocalStatic;
public:
    EvalEnvelop() 
    : m_typeInfo(NULL)
    , m_allocAddr(NULL)
    , m_islhs(false)
    , m_status(EE_SUCCESS) 
    , m_isEvalLocalStatic(false)
    {};
    ~EvalEnvelop() {}
    void setTypeInfo(TypeInfo *typeInfo)
    {
        m_typeInfo = typeInfo;
    }
    void setAllocAddr(VhAllocEEAddress *allocAddr)
    {
        m_allocAddr = allocAddr;
    }
    void setIsLhs(bool islhs)
    {
        m_islhs = islhs;
    }
    void setStatus(EEerrCode status)
    {
        m_status = status;
    }
    void setIsEvalLocalStatic(bool isEvalLocalStatic)
    {
        m_isEvalLocalStatic = isEvalLocalStatic;
    }
    TypeInfo *getTypeInfo()
    {
        return m_typeInfo;
    }
    VhAllocEEAddress *getAllocAddr()
    {
        return m_allocAddr;
    }
    bool getIsLhs()
    {
        return m_islhs;
    }
    EEerrCode &getStatus()
    {
        return m_status;
    }
    bool getIsEvalLocalStatic()
    {
        return m_isEvalLocalStatic;
    }
};

template <typename T1, ExprConstraintsType consType> class VhRangeEEAddress;
typedef VhRangeEEAddress<int64T,  intRangeConstraintType>  VhIntRangeEEAddress;
typedef VhRangeEEAddress<real64T, realRangeConstraintType> VhRealRangeEEAddress;

template <typename T1, uint32T arraySize, ExprConstraintsType consType> class TemplatizedExprConstraints;
                                                                                                           
template <typename T1>
class ExprRange
{
    friend class TemplatizedExprConstraints<T1, 8, constraintArrayType>;
    friend class TemplatizedExprConstraints<T1, 8, unConstraintArrayType>;
    friend class TemplatizedExprConstraints<T1, 1, intRangeConstraintType>;
    friend class TemplatizedExprConstraints<T1, 1, realRangeConstraintType>;

    T1        m_left;
    T1        m_right;
    VhDirType m_direction;

    ExprRange(T1 left, T1 right, VhDirType dir)
    : m_left     (left)
    , m_right    (right)
    , m_direction(dir)
    { }
    
    ExprRange()
    : m_direction(VH_ERROR_DIR)
    { }
    
    ExprRange(const ExprRange &rhs) {
        m_left      = rhs.m_left;
        m_right     = rhs.m_right;
        m_direction = rhs.m_direction;
    }

    ~ExprRange() { }

    ExprRange& operator = (const ExprRange &rhs) {
        m_left      = rhs.m_left;
        m_right     = rhs.m_right;
        m_direction = rhs.m_direction;
        return (*this);
    }
    
    bool isNullRange() const {
        return (m_direction == VH_TO) ? (m_left > m_right) 
                                      : (m_direction == VH_DOWNTO) ? (m_left < m_right) : true;
    }

    T1 getHigh() const { 
        return (m_direction == VH_DOWNTO) ? m_left : m_right; 
    }

    T1 getLow() const  { 
        return (m_direction == VH_DOWNTO) ? m_right : m_left; 
    }
    
    T1 getLeft() const {
        return m_left;
    }

    T1 getRight() const {
        return m_right;
    }

    T1 getLength() const {
        T1 h = getHigh();
        T1 l = getLow();

        return ((h >= l) ? (h - l + 1) : 0 );
    }

    VhDirType getDirType() const {
        return m_direction;
    }

    bool operator == (const ExprRange &rightOp) const {
        if ((m_left      != rightOp.m_left)
         || (m_right     != rightOp.m_right)
         || (m_direction != rightOp.m_direction)) {
            return false;
        }
        return true;
    }

    void browse() const
    {
        std::cout << "Left Bound : " << m_left << "    Right Bound : " << m_right 
                  << "    Direction : " << ((m_direction == VH_TO) ? "TO" : (m_direction == VH_DOWNTO)
                                                                            ? "DOWNTO" : "VH_ERROR_DIR") << std::endl;
    }
};

// Class to store index constraints for array expression
// We statically allocate for 8 dimensions for multidemensional arrays 
// Probabale area for memory optimization.
// NOTE: dimension is 1 based and not 0 based. 

class ExprEvaluatedConstraints
{
    friend class TypeInfo;
    friend class VhValueEEAddress;

protected:
    virtual ExprConstraintsType getConstraintsType()           const = 0;
    virtual bool                isNullRange()                  const = 0;
    virtual bool                isInvalidOrNullRange()         const = 0;
    virtual int64T              getNumberOfElements()          const = 0;
    virtual uint32T             getDimension()                 const = 0; 

public:
    virtual ~ExprEvaluatedConstraints() {}
    virtual bool                 isValidRange()                 const = 0;
    virtual VhValueEEAddress*    getValueAddr(VhAttrbType attrType, VhValueEEAddress *suffAddr) const = 0;
    virtual VhIntRangeEEAddress* getRangeAddr(VhAttrbType attrType, VhValueEEAddress *suffAddr) const = 0;
    virtual void                 getRangeFromLeftWithSize(uint32T dim, int64T size, int64T &left, int64T &right, VhDirType &dir) const = 0;
    virtual void                 browse(uint32T spaceCount = 0) const = 0;
};

template <typename T1, uint32T arraySize, ExprConstraintsType consType>
class TemplatizedExprConstraints : public ExprEvaluatedConstraints
{
    friend class TypeInfo;

public:
    static const uint32T             cm_MaxRange        = arraySize;
    static const ExprConstraintsType cm_ConstraintsType = consType;

//protected:
    uint32T        m_dimension;
    ExprRange<T1>  m_rangeArr[cm_MaxRange];

    virtual ExprConstraintsType getConstraintsType() const {return cm_ConstraintsType;}

    virtual bool isNullRange() const {
        for (uint32T n = 0; n < m_dimension; n++) {
            if (m_rangeArr[n].isNullRange() == true) {
                return true;
            }
        }
        return false;
    }

    virtual bool isInvalidOrNullRange() const {
        return (isNullRange() || !isValidRange());
    }

    virtual int64T getNumberOfElements() const {
        if (isInvalidOrNullRange()) {
            return 0;
        } else if (cm_ConstraintsType == unConstraintArrayType) {
            return 1;
        }
        int64T totalElement = 1;
        for (uint32T n = 0; n < m_dimension; n++) {
            JAG_ASSERT((m_rangeArr[n].m_direction == VH_TO) || (m_rangeArr[n].m_direction == VH_DOWNTO));
            totalElement *= m_rangeArr[n].getLength();
        }
        return totalElement;
    }

    virtual uint32T getDimension() const { 
        return m_dimension; 
    }
        
public:
    TemplatizedExprConstraints(uint32T dim, const T1 *left, const T1 *right, const VhDirType *dir)
    : m_dimension (dim)
    {
        for (uint32T n = 0; n < dim; n++) {
            m_rangeArr[n].m_left      = left [n];
            m_rangeArr[n].m_right     = right[n];
            m_rangeArr[n].m_direction = dir  [n];
        }
    }

    TemplatizedExprConstraints(const T1 left, const T1 right, const VhDirType dir)
    : m_dimension (1)
    {
        m_rangeArr[0].m_left      = left;
        m_rangeArr[0].m_right     = right;
        m_rangeArr[0].m_direction = dir;
    }

    TemplatizedExprConstraints(const TemplatizedExprConstraints &rhs) {
        m_dimension       = rhs.m_dimension;
        for (uint32T n = 0; n < m_dimension; n++) {
            m_rangeArr[n] = rhs.m_rangeArr[n];
        }
    }

    virtual ~TemplatizedExprConstraints() {}

    TemplatizedExprConstraints& operator = (const TemplatizedExprConstraints &rhs) {
        m_dimension       = rhs.m_dimension;
        for (uint32T n = 0; n < m_dimension; n++) {
            m_rangeArr[n] = rhs.m_rangeArr[n];
        }
        return (*this);
    }

    bool operator == (const TemplatizedExprConstraints &rightOp) const {
        if (m_dimension != rightOp.m_dimension) {
            return false;
        } else if (cm_ConstraintsType != rightOp.cm_ConstraintsType) {
            return false;
        } else if ((cm_ConstraintsType == unConstraintArrayType) && (rightOp.cm_ConstraintsType == unConstraintArrayType)) {
            return false;
        }
        for (uint32T n = 0; n < m_dimension; n++) {
            if (!(m_rangeArr[n] == rightOp.m_rangeArr[n])) {
                return false;
            }
        }
        return true;
    }

    virtual bool isValidRange() const {
        if (m_dimension == 0) {
            return false;
        }
        for (uint32T n = 0; n < m_dimension; n++) {
            if (m_rangeArr[n].m_direction == VH_ERROR_DIR) {
                return false;
            }
        }
        return true;
    }

    T1 left(uint32T dim) const { 
        JAG_ASSERT(dim <= m_dimension);
        return m_rangeArr[dim - 1].getLeft(); 
    }
        
    T1 right(uint32T dim) const { 
        JAG_ASSERT(dim <= m_dimension);
        return m_rangeArr[dim - 1].getRight(); 
    }

    T1 high(uint32T dim) const { 
        JAG_ASSERT(dim <= m_dimension);
        return m_rangeArr[dim - 1].getHigh(); 
    }

    T1 low(uint32T dim) const { 
        JAG_ASSERT(dim <= m_dimension);
        return m_rangeArr[dim - 1].getLow(); 
    }

    VhDirType getDirection(uint32T dim) const { 
        JAG_ASSERT(dim <= m_dimension);
        return m_rangeArr[dim - 1].getDirType(); 
    }
    
    T1 length(uint32T dim) const {
        JAG_ASSERT(dim <= m_dimension);
        return m_rangeArr[dim - 1].getLength();
    }

    void getRange(uint32T dim, T1 &left, T1 &right, VhDirType &dir) const {
        JAG_ASSERT(dim <= m_dimension);
        left  = m_rangeArr[dim - 1].getLeft();
        right = m_rangeArr[dim - 1].getRight();
        dir   = m_rangeArr[dim - 1].getDirType();
    }

    virtual VhValueEEAddress*    getValueAddr(VhAttrbType attrType, VhValueEEAddress *suffAddr) const;
    virtual VhIntRangeEEAddress* getRangeAddr(VhAttrbType attrType, VhValueEEAddress *suffAddr) const;

    virtual void getRangeFromLeftWithSize(uint32T dim, int64T size, int64T &left, int64T &right, VhDirType &dir) const {
        JAG_ASSERT(dim <= m_dimension);
        left  = (int64T)m_rangeArr[dim - 1].getLeft();
        dir   =         m_rangeArr[dim - 1].getDirType();
        right = (dir == VH_TO) ? (left + (size - 1)) : ((dir == VH_DOWNTO) ? (left - (size - 1)) : left);
    }

    virtual void browse(uint32T spaceCount = 0) const
    {
        for (uint32T j = 0; j < spaceCount; j ++) {
            std::cout << " ";
        }
        if ((cm_ConstraintsType == intRangeConstraintType) || (cm_ConstraintsType == realRangeConstraintType)) {
            std::cout << "Range Constraint" << std::endl;
        } else {
            std::cout << "Index Constraint" << std::endl;
        }
        for (uint32T i = 0; i < m_dimension; i++) {
            for (uint32T j = 0; j < spaceCount; j ++) {
                std::cout << " ";
            }
            std::cout << "Dimension[" << i + 1 << "] : ";
            m_rangeArr[i].browse();
        }
    }
};

template <typename T1, ExprConstraintsType consType>
class ExprRangeConstraints : public TemplatizedExprConstraints<T1, 1, consType>
{
private:
    ExprRangeConstraints()
    {
    }

public:
    virtual int64T    getNumberOfElements()          const { return 1; }

public:
    ExprRangeConstraints(T1 left, T1 right, VhDirType dir)
    : TemplatizedExprConstraints<T1, 1, consType>(left, right, dir)
    {
    }

    ExprRangeConstraints(const VhDecLit *pLeft, const VhDecLit *pRight, VhDirType pDir)
    : TemplatizedExprConstraints<T1, 1, consType>(const_cast<VhDecLit*>(pLeft)->getValue(),
                                                  const_cast<VhDecLit*>(pRight)->getValue(),
                                                  pDir)

    {
    }

    ExprRangeConstraints(const VhCharLit *pLeft, const VhCharLit *pRight, VhDirType pDir)
    : TemplatizedExprConstraints<T1, 1, consType>(const_cast<VhCharLit*>(pLeft)->getValue(),
                                                  const_cast<VhCharLit*>(pRight)->getValue(),
                                                  pDir)
    {
    }

    ExprRangeConstraints(const VhIdEnumLit *pLeft, const VhIdEnumLit *pRight, VhDirType pDir)
    : TemplatizedExprConstraints<T1, 1, consType>(const_cast<VhIdEnumLit*>(pLeft)->getValue(),
                                                  const_cast<VhIdEnumLit*>(pRight)->getValue(),
                                                  pDir)
    {
    }

    virtual ~ExprRangeConstraints() {}

    bool checkValueInRange(T1 d)
    {
        return (! ((d < TemplatizedExprConstraints<T1, 1, consType>::low(1)) 
                || (d > TemplatizedExprConstraints<T1, 1, consType>::high(1))));
    }
};

typedef TemplatizedExprConstraints<int64T,  8, constraintArrayType>     ExprBoundConstraints;
typedef TemplatizedExprConstraints<int64T,  8, unConstraintArrayType>   ExprUnBoundConstraints;
typedef ExprRangeConstraints      <int64T,     intRangeConstraintType>  ExprIntRangeConstraints;
typedef ExprRangeConstraints      <real64T,    realRangeConstraintType> ExprRealRangeConstraints;

extern ExprBoundConstraints* createConstraintFromUnconstrained(const ExprUnBoundConstraints *cons, uint64T size);
extern TypeInfo *vha_getTypeInfo(VhBase *pType, VhConstraint *pConstraint, bool bInsertIntoContainer, bool &isCreated, EvalEnvelop &env);

class RecordElemInfo 
{
    friend class TypeInfo;

    uint64T  m_offset;
    TypeInfo *m_typeInfo;
    uint32T  m_index;

public:
    RecordElemInfo(uint64T offset, TypeInfo *pTypeInfo, uint32T index) 
    : m_offset   (offset)
    , m_typeInfo (pTypeInfo)
    , m_index    (index)
    { }

    uint64T getOffset() const {
        return m_offset;
    }

    void setOffset(uint64T offset) {
        m_offset = offset;
    }

    TypeInfo* getTypeInfo() {
        return m_typeInfo;
    }

    void setTypeInfo(TypeInfo *pTypeInfo) {
        m_typeInfo = pTypeInfo;
    }

    uint32T getIndex() {
        return m_index;
    }

    void setIndex(uint32T index) {
        m_index = index;
    }

    static int32T compareRecElemOrder(const void *p1, const void *p2)
    {
        const RecordElemInfo *pRecInfo1 = *((RecordElemInfo **)p1);
        const RecordElemInfo *pRecInfo2 = *((RecordElemInfo **)p2);
        return ((pRecInfo1->m_index < pRecInfo2->m_index) ? -1 : 1);
    }
};

struct strCaseComparator 
{
    bool operator()(const char *lhs, const char *rhs) const {
        return vh_strcasecmp(lhs, rhs) < 0;
    }
};

class TypeInfo 
{
    friend class VhTypedEEAddress;
    friend class VhValueEEAddress;

#ifdef __JAG_EVAL_MEM_CHECK__
public:
    static uint32T            count;
#endif

private:
    uint64T                   m_size;
    VhBase                    *m_type;
    TypeInfo                  *m_typeInfo;
    ExprEvaluatedConstraints  *m_indexConstraint;
    // Club the following flags in one int32T like done in VhEEAddress
    bool                      m_isDeletable; 
    bool                      m_isConstrainedUptoThis; 
    bool                      m_isNonDeletableUptoThis;
    bool                      m_isAccessType;

    std::map<char*, RecordElemInfo*, strCaseComparator> m_recordInfoTable;

    void printSpace(int32T spaceCount) const;

    void setIsConstrainedUptoThis() {
        if (m_indexConstraint != NULL) {
            if ((m_typeInfo != NULL) && (m_typeInfo->m_isConstrainedUptoThis == true) && (m_indexConstraint->getConstraintsType() != unConstraintArrayType)) {
                m_isConstrainedUptoThis = true;
            } else if ((m_typeInfo == NULL) && (m_indexConstraint->getConstraintsType() != unConstraintArrayType)) {
                m_isConstrainedUptoThis = true;
            } else {
                m_isConstrainedUptoThis = false;
            }
        } else {
            if ((m_typeInfo != NULL) && (m_typeInfo->m_isConstrainedUptoThis == true)) {
                m_isConstrainedUptoThis = true;
            } else if ((m_typeInfo == NULL)) {
                if (m_recordInfoTable.size() == 0) {
                    m_isConstrainedUptoThis = true;
                } else {
                    m_isConstrainedUptoThis = true;
                    std::map<char*, RecordElemInfo*, strCaseComparator>::const_iterator it;
                    for (it = m_recordInfoTable.begin(); it != m_recordInfoTable.end(); ++it) {
                        if(((it->second)->getTypeInfo())->m_isConstrainedUptoThis == false) {
                            m_isConstrainedUptoThis = false;
                            break;
                        }
                    }
                }
            } else {
                m_isConstrainedUptoThis = false;
            }
        }
        return;
    }

    void setIsNonDeletableUptoThis() {
        if (m_isDeletable == true) {
            m_isNonDeletableUptoThis = false;
        } else if ((m_typeInfo == NULL) || (m_typeInfo->m_isNonDeletableUptoThis == true)) {
            m_isNonDeletableUptoThis = true;
        } else {
            m_isNonDeletableUptoThis = false;            
        }
    }

    void populateRecordElemOffsetTable(const TypeInfo &srcTypeInfo)
    {
        if (srcTypeInfo.m_recordInfoTable.size() > 0) {
            std::map<char *, RecordElemInfo *, strCaseComparator>::const_iterator it;
            for (it = srcTypeInfo.m_recordInfoTable.begin(); it != srcTypeInfo.m_recordInfoTable.end(); ++it) {
                RecordElemInfo *pRecordElemInfo = new RecordElemInfo((it->second)->getOffset(), 
                                                            (it->second)->getTypeInfo(), (it->second)->getIndex());
                char           *elemName        = it->first;
                insertToRecordInfoTable(elemName, pRecordElemInfo);
            }
        }
    }

    int64T getFlatIndex(int64T *indexArray, uint32T noDim, VhExpr *expr, bool showErr) const {
        if ((m_indexConstraint == NULL) || (m_indexConstraint->getConstraintsType() != constraintArrayType)) {
            return -1;
        } else {
            ExprBoundConstraints *t_indexConstraint = dynamic_cast<ExprBoundConstraints*>(m_indexConstraint);
            JAG_ASSERT(t_indexConstraint != NULL);
            int64T p         = 1;
            int64T i         = 0;
            int64T flatIndex = 0;
            for(uint32T lc = noDim; lc != 0; lc--) {
                if ((indexArray[lc - 1] < t_indexConstraint->low(lc))
                 || (indexArray[lc - 1] > t_indexConstraint->high(lc))) {
                    if (showErr == true) {
                        vha_errorAt(expr ? expr->getFileName() : const_cast<char*>(""), expr ? expr->getStartLineNumber() : 0, 446,
                            indexArray[lc - 1], t_indexConstraint->left(lc), 
                            t_indexConstraint->getDirection(lc) == VH_TO ? "to" : "downto", t_indexConstraint->right(lc));
                    }
                    return -1;
                }
                i          = ::abs((int)(t_indexConstraint->left(lc) - indexArray[lc - 1]));
                flatIndex += i * p;
                p         *= ::abs((int)(t_indexConstraint->left(lc) - t_indexConstraint->right(lc))) + 1;
            }
            return flatIndex;
        }
    }

    void copyConstraintInfo(const TypeInfo &srcTypeInfo, ExprEvaluatedConstraints *constraint)
    {
        ExprEvaluatedConstraints *t_constraint = constraint != NULL ? constraint : srcTypeInfo.m_indexConstraint;
        if ((t_constraint == NULL) || (t_constraint == constraint)) {
            m_indexConstraint = t_constraint;
            return;
        }
        if (t_constraint->getConstraintsType() == intRangeConstraintType) {
            m_indexConstraint = new ExprIntRangeConstraints(*(dynamic_cast<ExprIntRangeConstraints*>(srcTypeInfo.m_indexConstraint)));
        } else if (t_constraint->getConstraintsType() == realRangeConstraintType) {
            m_indexConstraint = new ExprRealRangeConstraints(*(dynamic_cast<ExprRealRangeConstraints*>(srcTypeInfo.m_indexConstraint)));
        } else if (t_constraint->getConstraintsType() == constraintArrayType) {
            m_indexConstraint = new ExprBoundConstraints(*(dynamic_cast<ExprBoundConstraints*>(srcTypeInfo.m_indexConstraint)));
        } else if (t_constraint->getConstraintsType() == unConstraintArrayType) {
            m_indexConstraint = new ExprUnBoundConstraints(*(dynamic_cast<ExprUnBoundConstraints*>(srcTypeInfo.m_indexConstraint)));
        } else {
            m_indexConstraint = NULL;
        }
    }

    bool isConstraintSameSize(const TypeInfo &newTypeInfo)
    {
        if (m_indexConstraint->getConstraintsType() == intRangeConstraintType) {
            if (!((*(dynamic_cast<ExprIntRangeConstraints*>(m_indexConstraint))) == (*(dynamic_cast<ExprIntRangeConstraints*>(newTypeInfo.m_indexConstraint))))) {
                return false;
            }
        } else if (m_indexConstraint->getConstraintsType() == realRangeConstraintType) {
            if (!((*(dynamic_cast<ExprRealRangeConstraints*>(m_indexConstraint))) == (*(dynamic_cast<ExprRealRangeConstraints*>(newTypeInfo.m_indexConstraint))))) {
                return false;
            }
        } else if (m_indexConstraint->getConstraintsType() == constraintArrayType) {
            if (!((*(dynamic_cast<ExprBoundConstraints*>(m_indexConstraint))) == (*(dynamic_cast<ExprBoundConstraints*>(newTypeInfo.m_indexConstraint))))) {
                return false;
            }
        } else if (m_indexConstraint->getConstraintsType() == unConstraintArrayType) {
            if (!((*(dynamic_cast<ExprUnBoundConstraints*>(m_indexConstraint))) == (*(dynamic_cast<ExprUnBoundConstraints*>(newTypeInfo.m_indexConstraint))))) {
                return false;
            }
        }
        return true;
    }

public:
    TypeInfo(VhBase *pType)
    : m_size           (8)
    , m_type           (pType)
    , m_typeInfo       (NULL)
    , m_indexConstraint(NULL)
    , m_isDeletable    (false)
    , m_isAccessType   (false)
    {
        setIsConstrainedUptoThis();
        setIsNonDeletableUptoThis();
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeTypeMapG[this] = this;
        std::cout << "line number : " << __LINE__ << " Pointer : " << this << std::endl;
#endif
    }

    TypeInfo(VhBase *pType, ExprEvaluatedConstraints *constraint, uint64T size, TypeInfo *eleTypeInfo)
    : m_size           (size == 0 ? (eleTypeInfo != NULL ? eleTypeInfo->m_size : 8) : size)
    , m_type           (pType)
    , m_typeInfo       (eleTypeInfo)
    , m_indexConstraint(constraint)
    , m_isDeletable    (false)
    , m_isAccessType   (false)
    {
        setIsConstrainedUptoThis();
        setIsNonDeletableUptoThis();
        if ((m_indexConstraint != NULL) && (m_typeInfo != NULL) && (m_typeInfo->m_isAccessType == false)) {
            m_size = m_indexConstraint->getNumberOfElements() * m_typeInfo->getSize();
        }
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeTypeMapG[this] = this;
        std::cout << "line number : " << __LINE__ << " Pointer : " << this << std::endl;
#endif
    }

    TypeInfo(VhBase *pType, ExprIntRangeConstraints &constraint)
    : m_size           (8)
    , m_type           (pType)
    , m_typeInfo       (NULL)
    , m_indexConstraint(&constraint)
    , m_isDeletable    (false)
    , m_isAccessType   (false)
    {
        setIsConstrainedUptoThis();
        setIsNonDeletableUptoThis();
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeTypeMapG[this] = this;
        std::cout << "line number : " << __LINE__ << " Pointer : " << this << std::endl;
#endif
    }

    TypeInfo(VhBase *pType, ExprRealRangeConstraints &constraint)
    : m_size           (8)
    , m_type           (pType)
    , m_typeInfo       (NULL)
    , m_indexConstraint(&constraint)
    , m_isDeletable    (false)
    , m_isAccessType   (false)
    {
        setIsConstrainedUptoThis();
        setIsNonDeletableUptoThis();
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeTypeMapG[this] = this;
        std::cout << "line number : " << __LINE__ << " Pointer : " << this << std::endl;
#endif
    }

    TypeInfo(const TypeInfo &srcTypeInfo)
    : m_size                 (srcTypeInfo.m_size)
    , m_type                 (srcTypeInfo.m_type)
    , m_isDeletable          (true)
    , m_isConstrainedUptoThis(srcTypeInfo.m_isConstrainedUptoThis) 
    , m_isAccessType         (srcTypeInfo.m_isAccessType)
    { 
        m_typeInfo = srcTypeInfo.m_typeInfo ? new TypeInfo(*(srcTypeInfo.m_typeInfo)) : NULL;
        copyConstraintInfo(srcTypeInfo, NULL);
        populateRecordElemOffsetTable(srcTypeInfo);
        setIsNonDeletableUptoThis();
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeTypeMapG[this] = this;
        std::cout << "line number : " << __LINE__ << " Pointer : " << this << std::endl;
#endif
    }

    TypeInfo(const TypeInfo &srcTypeInfo, VhBase *pType, ExprEvaluatedConstraints *constraint, uint64T size, TypeInfo *eleTypeInfo)
    : m_size         (size == 0 ? srcTypeInfo.m_size : size)
    , m_isDeletable  (true)
    , m_isAccessType (srcTypeInfo.m_isAccessType)
    { 
        m_type            = (pType       == NULL) ? srcTypeInfo.m_type     : pType;
        m_typeInfo        = (eleTypeInfo == NULL) ? srcTypeInfo.m_typeInfo : eleTypeInfo;
        copyConstraintInfo(srcTypeInfo, constraint);
        if ((m_indexConstraint != NULL) && (m_typeInfo != NULL) && (m_typeInfo->m_isAccessType == false)) {
            m_size = m_indexConstraint->getNumberOfElements() * m_typeInfo->getSize();
        }
        setIsConstrainedUptoThis();
        setIsNonDeletableUptoThis();
        populateRecordElemOffsetTable(srcTypeInfo);
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeTypeMapG[this] = this;
        std::cout << "line number : " << __LINE__ << " Pointer : " << this << std::endl;
#endif
    }

    ~TypeInfo()
    { 
#ifdef __JAG_EVAL_MEM_CHECK__
        count--; 
        eeTypeMapG.erase(this);
#endif
        if (m_indexConstraint) {
            delete m_indexConstraint;
            m_indexConstraint = NULL;
        }
        if (m_recordInfoTable.size() > 0) {
            std::map<char *, RecordElemInfo *, strCaseComparator>::const_iterator it;
            for (it = m_recordInfoTable.begin(); it != m_recordInfoTable.end(); ++it) {
                delete it->second;
            }
        }
    }

    int64T getNumberOfElements() const {
        return m_indexConstraint ? m_indexConstraint->getNumberOfElements() : 1;
    }

    uint32T getTotalDimensionsInHier() const {
        uint32T noOfDim = 0;
        if ((m_indexConstraint->getConstraintsType() == constraintArrayType)
         || (m_indexConstraint->getConstraintsType() == unConstraintArrayType)) {
            noOfDim = m_indexConstraint->getDimension();
        }
        return noOfDim + ((m_typeInfo != NULL) ? m_typeInfo->getTotalDimensionsInHier() : 0);
    }

    VhValueEEAddress* createEEAddressFromTypeInfo(bool bUseDefaultValue, EvalEnvelop &env);

    TypeInfo* getTypeInfoHierWithCopyOfTemp() {
        if ((m_isConstrainedUptoThis == true) && (m_isNonDeletableUptoThis == true)) {
            return this;
        } else {
            TypeInfo *eleTypeInfo  = m_typeInfo ? m_typeInfo->getTypeInfoHierWithCopyOfTemp() : NULL;
            TypeInfo *copyTypeInfo = new TypeInfo(*this, NULL, NULL, 0, eleTypeInfo);
            return copyTypeInfo;
        }
    }

    TypeInfo* merge(const TypeInfo &mergeFromTypeInfo) 
    {
        if (m_recordInfoTable.size() > 0) {
            JAG_ASSERT(mergeFromTypeInfo.m_recordInfoTable.size() > 0);
            if (!((m_isConstrainedUptoThis == true) && (m_isNonDeletableUptoThis == true))) {
                TypeInfo       *retTypeInfo        = new TypeInfo(*this);
                uint32T        elemCount           = 0;
                uint32T        elemCountSrc        = 0;
                RecordElemInfo **arrRecElemInfo    = retTypeInfo->getRecordElemInfoArray(elemCount);
                RecordElemInfo **arrRecElemInfoSrc = mergeFromTypeInfo.getRecordElemInfoArray(elemCountSrc);
                JAG_ASSERT(elemCount == elemCountSrc);
                for (uint32T i = 0; i < elemCount; i ++) {
                    if ((arrRecElemInfo[i]->m_typeInfo)->m_isConstrainedUptoThis == false) {
                        arrRecElemInfo[i]->m_typeInfo = (arrRecElemInfo[i]->m_typeInfo)->merge(*(arrRecElemInfoSrc[i]->m_typeInfo));
                    }
                }
                retTypeInfo->regenerateRecOffsetTable();
                TypeInfo::deleteRecordElemInfoArray(arrRecElemInfo);
                TypeInfo::deleteRecordElemInfoArray(arrRecElemInfoSrc);
                return retTypeInfo;
            } else {
                return this;
            }
        }
        TypeInfo *mergedTypeInfo = NULL;
        if ((m_typeInfo != NULL) && (mergeFromTypeInfo.m_typeInfo != NULL)) {
            mergedTypeInfo = m_typeInfo->merge(*(mergeFromTypeInfo.m_typeInfo));
        }

        JAG_ASSERT(!(((m_typeInfo == NULL) && (mergeFromTypeInfo.m_typeInfo != NULL)) 
              || ((m_typeInfo != NULL) && (mergeFromTypeInfo.m_typeInfo == NULL))));

        ExprBoundConstraints *mergeConstraint = NULL;
        if ((this->getIndexConstraints() == NULL) && (mergeFromTypeInfo.getIndexConstraints() != NULL)) {
            mergeConstraint = new ExprBoundConstraints(*(dynamic_cast<ExprBoundConstraints*>(mergeFromTypeInfo.m_indexConstraint)));
        }
        TypeInfo *retTypeInfo = new TypeInfo(*this, NULL, mergeConstraint, 0, mergedTypeInfo);
        return retTypeInfo;
    }

    // Used only for multi-D array.
    // No need to copy element offset table for records.
    TypeInfo *createSubArrayTypeInfo()
    {
        JAG_ASSERT((m_indexConstraint != NULL) 
            && (m_indexConstraint->getDimension()  > 1)
            && (m_indexConstraint->getConstraintsType() == constraintArrayType));
        ExprBoundConstraints *t_indexConstraint = dynamic_cast<ExprBoundConstraints*>(m_indexConstraint);
        JAG_ASSERT(t_indexConstraint != NULL);
        uint32T   dim                                      = t_indexConstraint->getDimension() - 1;
        int64T    left [ExprBoundConstraints::cm_MaxRange] = {0};
        int64T    right[ExprBoundConstraints::cm_MaxRange] = {0};
        VhDirType dir  [ExprBoundConstraints::cm_MaxRange] = {VH_ERROR_DIR};
        for (uint32T i = 1; i < t_indexConstraint->getDimension(); i++) {
            left[i - 1]  = t_indexConstraint->left(i + 1);
            right[i - 1] = t_indexConstraint->right(i + 1);
            dir[i - 1]   = t_indexConstraint->getDirection(i + 1);
        }
        ExprBoundConstraints *indexConstraint = new ExprBoundConstraints(dim, left, right, dir);
        TypeInfo *pSubArrayTypeInfo           = new TypeInfo(NULL, indexConstraint, 0, m_typeInfo);
        return pSubArrayTypeInfo;
    }

    ExprEvaluatedConstraints *getFirstDimensionIndCons()
    {
        ExprBoundConstraints *t_indexConstraint = dynamic_cast<ExprBoundConstraints*>(m_indexConstraint);
        JAG_ASSERT(t_indexConstraint != NULL);
        int64T    left [ExprBoundConstraints::cm_MaxRange] = {0};
        int64T    right[ExprBoundConstraints::cm_MaxRange] = {0};
        VhDirType dir  [ExprBoundConstraints::cm_MaxRange] = {VH_ERROR_DIR};
        left [0] = t_indexConstraint->left(1);
        right[0] = t_indexConstraint->right(1);
        dir  [0] = t_indexConstraint->getDirection(1);
        ExprBoundConstraints *indexConstraint = new ExprBoundConstraints(1, left, right, dir);
        return indexConstraint;
    }

    uint64T getSize() const {
        return m_size;
    }

    void setSize(uint64T size) {
        m_size = size;
    }

    void setType(VhBase *type) {
        m_type = type;
    }

    VhBase *getType() const {
        return m_type;
    }

    TypeInfo *getTypeInfo() const {
        return m_typeInfo;
    }

    bool isIntegerType() const;

    bool isIntegerSubType() const;

    bool isTimeType() const;

    bool isValidConstraint() const {
        return (((m_indexConstraint != NULL) && (m_indexConstraint->isValidRange() == true)) ? true : false);
    }

    bool isNullConstraint() const {
        return (((m_indexConstraint != NULL) && (m_indexConstraint->isNullRange() == true)) ? true : false);
    }

    bool ifContainsAccessTypeInHier() const {
        if (m_typeInfo == NULL) {
            if (m_recordInfoTable.size() > 0) {
                std::map<char*, RecordElemInfo*, strCaseComparator>::const_iterator it;
                for (it = m_recordInfoTable.begin(); it != m_recordInfoTable.end(); ++it) {
                    if(((it->second)->getTypeInfo())->m_isAccessType == true) {
                        return true;
                    }
                }
            }
            return m_isAccessType;
        } else {
            return m_isAccessType || m_typeInfo->ifContainsAccessTypeInHier();
        }
    }

    bool ifContainsNullConstraintInHier() const {
        if ((m_indexConstraint != NULL)
         && (m_indexConstraint->getConstraintsType() == constraintArrayType)
         && (m_indexConstraint->isNullRange() == true)) {
            return true;
        } else if (m_typeInfo != NULL) {
            return m_typeInfo->ifContainsNullConstraintInHier();
        } else {
            return false;
        }
    }

    bool ifContainsInvalidConstraintInHier() const {
        if ((m_indexConstraint != NULL) && (m_indexConstraint->isValidRange() == false)) {
            return true;
        } else if (m_typeInfo != NULL) {
            return m_typeInfo->ifContainsInvalidConstraintInHier();
        } else {
            return false;
        }
    }

    ExprBoundConstraints *getIndexConstraints() const {
        return (((m_indexConstraint != NULL) && (m_indexConstraint->getConstraintsType() == constraintArrayType)) ? (ExprBoundConstraints*)m_indexConstraint : NULL);
    }

    ExprEvaluatedConstraints* getConstraintInformation(ExprConstraintsType &consType) const {
        if (m_indexConstraint != NULL) {
            consType = m_indexConstraint->getConstraintsType();
        }
        return m_indexConstraint;
    }

    bool isDeletable() const {
        return m_isDeletable;
    }

    void setDeletable(bool value) {
        m_isDeletable = value;
        setIsNonDeletableUptoThis();    
    }

    bool getIsAccessType() {
        return m_isAccessType;
    }

    void setIsAccessType(bool val) {
        m_isAccessType = val;
    }

    bool getIsFullyConstrained() {
        return (((m_indexConstraint != NULL) && (m_indexConstraint->getConstraintsType() == constraintArrayType)) ? true : false);
    }
    
    bool getIsUnConstrained() {
        return (((m_indexConstraint != NULL) && (m_indexConstraint->getConstraintsType() == unConstraintArrayType)) ? true : false);
    }

    bool getIsConstrainedUptoThis() {
        return m_isConstrainedUptoThis;
    }
    
    ExprUnBoundConstraints *getUnConsRange() const {
        return (((m_indexConstraint != NULL) && (m_indexConstraint->getConstraintsType() == unConstraintArrayType)) ? (ExprUnBoundConstraints*)m_indexConstraint : NULL);
    }

    void insertToRecordInfoTable(char *elemName, RecordElemInfo *pRecordInfo) {
        m_recordInfoTable[elemName] = pRecordInfo;
        if (pRecordInfo->getTypeInfo()->getIsConstrainedUptoThis() == false) {
            m_isConstrainedUptoThis = false;
        }
    }
    
    RecordElemInfo *getRecordInfo(char *elemName) {
        std::map<char *, RecordElemInfo *, strCaseComparator>::iterator it;
        it = m_recordInfoTable.find(elemName);
        if(it == m_recordInfoTable.end()) {
            return NULL;
        }
        return it->second;
    }

    void regenerateRecOffsetTable();

    bool isSameSize(const TypeInfo &newTypeInfo) {
        if ((m_indexConstraint == NULL) && (newTypeInfo.m_indexConstraint == NULL)) {
            return true;
        } else if ((m_indexConstraint == NULL) || (newTypeInfo.m_indexConstraint == NULL)) {
            return false;
        }
        bool consCheck = isConstraintSameSize(newTypeInfo);
        if (consCheck == false) {
            return false;
        }

        if ((m_typeInfo == NULL) && (newTypeInfo.m_typeInfo == NULL)) {
            return true;
        } else if ((m_typeInfo == NULL) || (newTypeInfo.m_typeInfo == NULL)) {
            return false;
        }
        return m_typeInfo->isSameSize(*(newTypeInfo.m_typeInfo));
    }

    // dontCheckUnConsType is not currently not reqd. Currently its always true.
    // This is kept for future reuse of this function.
    bool isMatchingSize(const TypeInfo &newTypeInfo, bool dontCheckUnConsType) {
        if (m_recordInfoTable.size() > 0) {
            uint32T        elemCount1        = 0;
            uint32T        elemCount2        = 0;
            RecordElemInfo **arrRecElemInfo1 = this->getRecordElemInfoArray(elemCount1);
            RecordElemInfo **arrRecElemInfo2 = newTypeInfo.getRecordElemInfoArray(elemCount2);
            JAG_ASSERT(elemCount1 == elemCount2);
            // Safety Check
            if (elemCount1 != elemCount2) {
                return false;
            }
            for (uint32T i = 0; i < elemCount1; i ++) {
                JAG_ASSERT (arrRecElemInfo1[i]->m_typeInfo != NULL);
                JAG_ASSERT (arrRecElemInfo2[i]->m_typeInfo != NULL);
                if ((arrRecElemInfo1[i]->m_typeInfo)->isMatchingSize(*(arrRecElemInfo2[i]->m_typeInfo), dontCheckUnConsType) == false) {
                    return false;
                }
            }
            return true;
        }

        if ((m_indexConstraint == NULL) && (newTypeInfo.m_indexConstraint == NULL)) {
            return true;
        } else if ((m_indexConstraint == NULL) || (newTypeInfo.m_indexConstraint == NULL)) {
            return false;
        }

        ExprConstraintsType constraintType1 = m_indexConstraint->getConstraintsType();
        ExprConstraintsType constraintType2 = newTypeInfo.m_indexConstraint->getConstraintsType();

        if (constraintType1 == intRangeConstraintType) {
            // Safety check.
            if (constraintType2 != intRangeConstraintType) {
                return false;
            }
            if (isConstraintSameSize(newTypeInfo) == false) {
                return false;
            }
        } else if (constraintType1 == realRangeConstraintType) {
            // Safety check.
            if (constraintType2 != realRangeConstraintType) {
                return false;
            }
            if (isConstraintSameSize(newTypeInfo) == false) {
                return false;
            }
        } else if ((constraintType1 == unConstraintArrayType)
                || (constraintType2 == unConstraintArrayType)) {
            // Current use of this function is to match subTypeInd of external name with
            // that of its actual. In this usage dontCheckUnConsType will always be true.
            // If in future dontCheckUnConsType == false  functionality is reqd, we need 
            // to uncomment the following lines of code.
            //if (dontCheckUnConsType == false) {
                //if (constraintType1 != constraintType2) {
                    //return false;
                //}
            //}
        } else if (getNumberOfElements() != newTypeInfo.getNumberOfElements()) {
            return false;
        }

        if ((m_typeInfo == NULL) && (newTypeInfo.m_typeInfo == NULL)) {
            return true;
        } else if ((m_typeInfo == NULL) || (newTypeInfo.m_typeInfo == NULL)) {
            return false;
        }
        return m_typeInfo->isMatchingSize(*(newTypeInfo.m_typeInfo), dontCheckUnConsType);
    }

    //EEAddrExprType vha_getEEAddrTypeFromTypeInfo()
    //{
    //    // Find the type of expression in perspective of EEAddress
    //    // from the information stored in TypeInfo.

    //    ExprConstraintsType consType = (m_indexConstraint == NULL) ? errorConstraintType : m_indexConstraint->getConstraintsType();
    //    EEAddrExprType    typeOfExpr = (consType == errorConstraintType) 
    //                                   ? (m_recordInfoTable.empty() == true) 
    //                                     ? Scalar 
    //                                     : Record 
    //                                   : ((consType == intRangeConstraintType) || (consType == realRangeConstraintType)) 
    //                                     ? Scalar 
    //                                     : Array;
    //    return typeOfExpr;
    //}

    EEAddrExprType vha_getEEAddrTypeFromTypeInfo()
    {
        if (m_isAccessType == true) {
            return AccessType;
        }
        /*
         * 27 July 2016 : Subhadeep
         * Small change is required in this function for non-static range in array type declaration
         * Also changing ternary operator ?: to if-else for better readability
         * Testcase: vhdl_new_expr_eval/nonStatRangeInArrayDecl[1 to 3]
         * Explanation: In case we have an array type declaration whose range in non-static.
         * In that case m_indexConstraint is evaluated to NULL. This function earlier was detecting 
         * the type to be scalar. So introducing a check that if m_indexConstraint is NULL but
         * m_typeInfo is non-NULL, then return ErrorType as the EEAddrExprType. In that case we should 
         * not create EEAddress from this typeInfo.
         */ 
        EEAddrExprType typeOfExpr = ErrorType;
        if (m_indexConstraint != NULL) {
            ExprConstraintsType consType = m_indexConstraint->getConstraintsType();
            if (consType != errorConstraintType) {
                if ((consType == intRangeConstraintType) || (consType == realRangeConstraintType)) {
                    typeOfExpr = Scalar;
                } else {
                    typeOfExpr = Array;
                }
            }
        } else if (m_typeInfo == NULL) {
            if (m_recordInfoTable.empty() == true) {
                typeOfExpr = Scalar;
            } else {
                typeOfExpr = Record;
            }
        }
        return typeOfExpr;
    }

    bool is1DArrayOfScalars()
    {
        // Records, and Scalars without range constraint will return from here.
        if (m_indexConstraint == NULL) {
            return false;
        }

        // evrything except constrained array (for example scalar with range constraint, 
        // unconstrained array etc) will return from here
        if (m_indexConstraint->getConstraintsType() != constraintArrayType) {
            return false;
        }

        //multi-D array will return from here
        if (m_indexConstraint->getDimension() != 1) {
            return false;
        }

        //array of array will return from here.
        if (m_typeInfo->m_typeInfo != NULL) {
            return false;
        }

        //array of records will return from here.
        if ((m_typeInfo->m_recordInfoTable).empty() == false) {
            return false;
        }
        return true;
    }

    bool isRealType()
    {
        ExprConstraintsType consType = (m_indexConstraint == NULL) ? errorConstraintType : m_indexConstraint->getConstraintsType();
        if (consType == realRangeConstraintType) {
            return true;
        } else if (m_typeInfo != NULL) {
            return m_typeInfo->isRealType();
        } else {
            return false;
        }
    }

    bool isOneDArray(bool isBitString) const;

    RecordElemInfo** getRecordElemInfoArray(uint32T &elemCount) const {
        if (m_recordInfoTable.empty()) {
            return NULL;
        }
        std::map<char*, RecordElemInfo*, strCaseComparator>::const_iterator it;
        elemCount = m_recordInfoTable.size();
        RecordElemInfo **arrRecElemInfo = new RecordElemInfo* [elemCount];
        uint32T count = 0;
        for (it = m_recordInfoTable.begin(); it != m_recordInfoTable.end(); ++it) {
            arrRecElemInfo[count ++] = it->second;
        }
        qsort(arrRecElemInfo, elemCount, sizeof(RecordElemInfo*), RecordElemInfo::compareRecElemOrder);
        return arrRecElemInfo;
    }

    void browse() const;

    static void cleanTypeInfoTree(TypeInfo *pTypeInfo) {
        if (pTypeInfo == NULL) {
            return;
        } else {
            TypeInfo::cleanTypeInfoTree(pTypeInfo->m_typeInfo);
            vha_deleteTypeInfo(pTypeInfo);
        }
    }


    static void deleteRecordElemInfoArray(RecordElemInfo **&arrRecElemInfo) {
        delete [] arrRecElemInfo;
        arrRecElemInfo = NULL;
    }

    static void DeleteObject(TypeInfo *&tTypeInfo) {
        tTypeInfo->setDeletable(true);
        vha_deleteTypeInfo(tTypeInfo);
    }
    //static void printLiveTypeInfos();
};

    // BIT(0) : isRealValue
    // Bit(1) : isNotDeletable. 
    //          NOTE: The VhEEAddress-es that are stored in a container in any scope 
    //                such as Architecture, Process etc, will have this bit set. In the destructor 
    //                of these scopes we will free the memory for m_ptr and m_partStat by iterating 
    //                through the container.  
    // Bit(2) :     0 signifies delete the allocated addresses
    //              1 signifies do not delete the allocated addresses
#define EEADDR_REAL                   0x00000001
#define EEADDR_NOT_DEL                0x00000002
#define EEADDR_NOT_DEL_ALLOC_SPACE    0x00000004
#define EEADDR_OUT_INOUT_PORT         0x00000008
#define EEADDR_RANGE                  0x00000010
#define EEADDR_OPEN                   0x00000020
#define EEADDR_FILE                   0x00000040
#define EEADDR_ALLOC                  0x00000080
#define EEADDR_VALUE                  0x00000100
#define EEADDR_NEXT                   0x10000000
#define EEADDR_EXIT                   0x20000000
#define EEADDR_NULL_RANGE             0x40000000
#define EEADDR_NULL_BUFFER            0x80000000
#define EEADDR_NULL_STRING            0x01000000
#define EEADDR_MALLOC_FAILED          0x02000000
#define EEADDR_CONTAINS_ALLOCADDR_PTR 0x08000000

#define HANDLE_MALLOC_FAIL_M_PTR \
    setProperty(EEADDR_MALLOC_FAILED, true); \
    m_partStat = NULL; \
    m_allocatedSize = ZERO_64_BIT; 
    //vha_error(522, "System malloc failed");

#define HANDLE_MALLOC_FAIL_M_PART_STAT \
    free(m_ptr); \
    m_ptr = NULL; \
    m_allocatedSize = ZERO_64_BIT; \
    setProperty(EEADDR_MALLOC_FAILED, true); 
    //vha_error(522, "System malloc failed");

#define CHAR_TYPE_FIRST_BIT_ONE    0x80

extern VhValueEEAddress vhGi_nullEEAddressForAllocType;

class VhEEAddress 
{
#ifdef __JAG_EVAL_MEM_CHECK__
public:
    static uint32T count;
#endif

protected:
    uint32T        m_flags;

public:
    
    VhEEAddress(uint32T flags)
    : m_flags (flags)
    {
#ifdef __JAG_EVAL_MEM_CHECK__
        count++;
        eeAddrMapG[this] = this;
#endif
    }

    virtual ~VhEEAddress() 
    {
#ifdef __JAG_EVAL_MEM_CHECK__
        count--;
        eeAddrMapG.erase(this);
#endif
    }

    void setProperty(uint32T flag, bool status) {
        m_flags = (status == true) ? (m_flags | flag) : (m_flags & (~(flag)));
    }

    bool getProperty(uint32T flag) const {
        return ((m_flags & flag) == 0) ? false : true;
    }

    static void DeleteObject(VhEEAddress *&tEeAddr) {
        if ((tEeAddr != NULL)
         && (tEeAddr != (VhEEAddress *)(&vhGi_nullEEAddressForAllocType))) {
            tEeAddr->setProperty(EEADDR_NOT_DEL, false);
            if (tEeAddr->getProperty(EEADDR_OUT_INOUT_PORT) == false) {
                tEeAddr->setProperty(EEADDR_NOT_DEL_ALLOC_SPACE, false);
            }
            tEeAddr = vha_deleteEEAddress(tEeAddr);
        }
    }

    template<typename T1>
    static T1 pow(T1 base, int64T power)
    {
        T1 zeroVal = static_cast<T1>(0);
        T1 oneVal  = static_cast<T1>(1);

        if ((base == oneVal) || (power == ZERO_64_BIT)) {
            return oneVal;
        } else if (base == zeroVal) {
            return zeroVal;
        }

        uint64T absP = (power < 0) ? -power : power;
        T1 tempResult = base;

        while ((absP & ONE_64_BIT) == ZERO_64_BIT) {
            tempResult *= tempResult;
            absP      >>= 1;
        }

        T1 result = tempResult;

        while (true) {
            absP >>= 1;
            if (absP == 0) {
                break;
            } else {
                tempResult *= tempResult;
                if ((absP & ONE_64_BIT) == ONE_64_BIT) {
                    result *= tempResult;
                }
            }
        }

        // For the case of negative power with positive int64T base
        // this function will return zero
        if (power < 0) {
            result = oneVal / result;
        }

        result = (result == zeroVal) ? (static_cast<T1>(std::numeric_limits<real64T>::min())) : result;
        return result;
    }


    virtual VhExpr* createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const = 0;
    virtual char*   createVerilogExpression(const char *dutName) const = 0;
    virtual void    browse() const;
};

class VhTypedEEAddress : public VhEEAddress
{
protected:
    // Populated type information of the expression. This structure is made to 
    // avoid most of the type related searching during evaluation.
    TypeInfo *m_typeInfo;

public:
    
    VhTypedEEAddress(uint32T flags)
    : VhEEAddress(flags)
    , m_typeInfo (NULL)
    {
    }

    virtual ~VhTypedEEAddress() {}

    TypeInfo *getTypeInfo() const {
        return m_typeInfo;
    }

    void setTypeInfo(const TypeInfo *pTypeInfo) {
        m_typeInfo = pTypeInfo ? (const_cast<TypeInfo*>(pTypeInfo))->getTypeInfoHierWithCopyOfTemp() : NULL;
    }
    
    void cloneTypeInfo() {
        if (m_typeInfo != NULL) {
            TypeInfo *pNewTypeInfo = new TypeInfo(*m_typeInfo);
            TypeInfo::cleanTypeInfoTree(m_typeInfo);
            m_typeInfo = pNewTypeInfo;
        }
    }

    bool getIsUnConstrained() const {
        return (((m_typeInfo != NULL) && (m_typeInfo->getIsConstrainedUptoThis() == false)) ? true : false);
    }

    ExprBoundConstraints *getIndexConstraints() const {
        return m_typeInfo ? m_typeInfo->getIndexConstraints() : NULL;
    }

    int64T getFlatIndex(int64T *indexArray, uint32T noDim, VhExpr *expr, bool showErr) const {
        return m_typeInfo ? m_typeInfo->getFlatIndex(indexArray, noDim, expr, showErr) : -1;
    }

    bool isSameSize(const VhTypedEEAddress &newEEAddr) {
        //if ((m_typeInfo == NULL) || (newEEAddr.m_typeInfo == NULL)) {
        //    return true;
        //}
        if (m_typeInfo->getUnConsRange() || newEEAddr.m_typeInfo->getUnConsRange()) {
            return false;
        }
        return m_typeInfo->isSameSize(*(newEEAddr.m_typeInfo));
    }

    virtual void browse() const;
};

class VhValueEEAddress : public VhTypedEEAddress
{
    friend class VhAllocEEAddress;
private:
    //Memory chunk to hold evaluated expression value
    char    *m_ptr;
    
    // m_allocatedSize is in bytes. m_allocatedSize = (no of elemets in terms of scalar value) * 8
    uint64T m_allocatedSize;
    
    // The valid portion of m_partStat will be from m_partStatInfoStart and for 
    // (no of elements / 8) + (no of elements % 8) bits.
    // The ith bit of m_partStat represents if the data in (m_ptr + (i - m_partStatInfoStart) * 8) 
    // is static or not.
    //  0 => Nonstatic, 1 => static
    uint8T  *m_partStat;

    // Offset in the first byte of m_partStat from which valid data start. This will be useful in 
    // creating VhValueEEAddress using existing one by reusing chunk of existing VhValueEEAddress. 
    // We will set the m_partStat of the new VhValueEEAddress to the valid byte of the m_partStat 
    // of the VhValueEEAddress from which we are creating, and set this value to the correct offset. 
    uint64T m_partStatInfoStart;

public:
    
    VhValueEEAddress(int64T x)
    : VhTypedEEAddress    (EEADDR_VALUE)
    , m_allocatedSize     (sizeof(int64T))
    , m_partStatInfoStart (ZERO_64_BIT)
    {
        m_ptr               = (char *) malloc(sizeof(int64T));
        *(int64T*)m_ptr     = x;
        m_partStat          = (uint8T *) malloc(sizeof(uint8T));
        *m_partStat         = CHAR_TYPE_FIRST_BIT_ONE;
        //std::cout << "count is VhValueEEAddress(int64T x) : " << count << std::endl;
    }
    
    VhValueEEAddress(real64T x)
    : VhTypedEEAddress    (EEADDR_VALUE | EEADDR_REAL)
    , m_allocatedSize     (sizeof(real64T))
    , m_partStatInfoStart (ZERO_64_BIT)
    {
        m_ptr               = (char *) malloc(sizeof(real64T));
        *(real64T*)m_ptr    = x;
        m_partStat          = (uint8T *) malloc(sizeof(uint8T));
        *m_partStat         = CHAR_TYPE_FIRST_BIT_ONE;
        //std::cout << "count is VhValueEEAddress(double x) : " << count << std::endl;
    }

    VhValueEEAddress(const char *str, EvalEnvelop &env)
    : VhTypedEEAddress    (EEADDR_VALUE)
    , m_allocatedSize     (ZERO_64_BIT)
    , m_partStatInfoStart (ZERO_64_BIT)
    {
        m_ptr      = NULL;
        m_partStat = NULL;
        bool     isCreated    = false;
        TypeInfo *strTypeInfo = vha_getTypeInfo(vhGi_stringSTD, NULL, true, isCreated, env);
        if (str != NULL) {
            uint64T  strSize = strlen(str);
            m_allocatedSize  = strSize * 8;
            ExprBoundConstraints *constraint = createConstraintFromUnconstrained(strTypeInfo->getUnConsRange(), strSize);
            TypeInfo             *nTypeInfo  = new TypeInfo(*strTypeInfo, NULL, constraint, 0, NULL);
            setTypeInfo(nTypeInfo);
            if ((m_allocatedSize & 0xFFFFFFFF00000000ULL) == 0) {
                m_ptr = (char *) malloc(m_allocatedSize);
            }
            if (m_ptr == NULL) {
                HANDLE_MALLOC_FAIL_M_PTR
            } else {
                uint64T partStatInfoSize = getPartStatInfoSize();
                m_partStat               = (uint8T *) malloc(partStatInfoSize);
                if (m_partStat == NULL) {
                    HANDLE_MALLOC_FAIL_M_PART_STAT
                } else {
                    memset(m_partStat, 0xFF, partStatInfoSize);
                    for (uint64T i = 0; i < strSize; i++) {
                        *(uint64T*)(m_ptr + (i << 3)) = (uint64T)str[i];
                    }
                    uint64T bitsInLastBlock = strSize & 0x7;
                    if (bitsInLastBlock != 0) {
                        m_partStat[partStatInfoSize - 1] <<= (8 - bitsInLastBlock);
                    }
                }
            }
        } else {
            setTypeInfo(strTypeInfo);
        }
    }

    VhValueEEAddress(uint64T size, TypeInfo *pTypeInfo)
    : VhTypedEEAddress    (EEADDR_VALUE)
    , m_allocatedSize     (size)
    , m_partStatInfoStart (ZERO_64_BIT)
    {
        setTypeInfo(pTypeInfo);

        m_ptr      = NULL;
        m_partStat = NULL;
        if (size != 0) {
            if ((size & 0xFFFFFFFF00000000ULL) == 0) {
                m_ptr = (char *) malloc(size);
            }
            if (m_ptr == NULL) {
                HANDLE_MALLOC_FAIL_M_PTR
            } else {
                uint64T partStatInfoSize = getPartStatInfoSize();
                m_partStat               = (uint8T *) malloc(partStatInfoSize);
                if (m_partStat == NULL) {
                    HANDLE_MALLOC_FAIL_M_PART_STAT
                } else {
                    memset(m_ptr, 0, m_allocatedSize);
                    memset(m_partStat, 0, partStatInfoSize);
                }
            }
        }

        if ((m_typeInfo != NULL) && (m_typeInfo->isRealType() == true)) {
            setProperty(EEADDR_REAL, true);
        }
    }

    VhValueEEAddress(const VhValueEEAddress &sourceAddr)
    : VhTypedEEAddress(sourceAddr.m_flags & (~(EEADDR_NOT_DEL | EEADDR_NOT_DEL_ALLOC_SPACE)))
    , m_allocatedSize (sourceAddr.m_allocatedSize)
    {
        m_ptr               = (char *) malloc(sourceAddr.m_allocatedSize);
        if (m_ptr == NULL) {
            HANDLE_MALLOC_FAIL_M_PTR
        } else {
            m_partStat          = (uint8T *) malloc(sourceAddr.getPartStatInfoSizeWithOffset());
            if (m_partStat == NULL) {
                HANDLE_MALLOC_FAIL_M_PART_STAT
            } else {
                memcpy(m_ptr, sourceAddr.m_ptr, m_allocatedSize);
                memcpy(m_partStat, sourceAddr.m_partStat, sourceAddr.getPartStatInfoSizeWithOffset());
                m_partStatInfoStart = sourceAddr.m_partStatInfoStart;
                if (sourceAddr.getProperty(EEADDR_CONTAINS_ALLOCADDR_PTR) == false) {
                    m_typeInfo      = new TypeInfo(*sourceAddr.m_typeInfo);
                }
            }
        }
    }

    VhValueEEAddress(const VhValueEEAddress &sourceAddr, uint64T targetStart, uint64T targetSize, TypeInfo *pTypeInfo, VhExpr *expr)
    : VhTypedEEAddress    ((sourceAddr.m_flags & (~EEADDR_NOT_DEL)) | EEADDR_NOT_DEL_ALLOC_SPACE)
    , m_allocatedSize     (targetSize)
    {
        if ((targetStart + targetSize) > sourceAddr.m_allocatedSize) {
            if (sourceAddr.getAllocatedSize() != 0) {
                vha_errorAt(expr ? expr->getFileName() : const_cast<char*>(""), expr ? expr->getStartLineNumber() : 0, 272);
            }
            setTypeInfo(pTypeInfo);
            m_ptr                  = NULL;
            m_partStat             = NULL;
            m_partStatInfoStart    = ZERO_64_BIT;
            m_allocatedSize        = ZERO_64_BIT;
        } else {
            setTypeInfo(pTypeInfo);
            m_ptr                  = sourceAddr.m_ptr + targetStart;
            uint64T noOfElemOffset = targetStart    / 8;
            uint64T byteOffset     = noOfElemOffset / 8;
            uint64T bitOffset      = (noOfElemOffset % 8) + sourceAddr.m_partStatInfoStart;

            if (bitOffset / 8 == 1) {
                byteOffset ++;
                bitOffset = bitOffset % 8;
            }
            m_partStat          = sourceAddr.m_partStat + byteOffset;
            m_partStatInfoStart = bitOffset;
        }
        if ((m_typeInfo != NULL) && (m_typeInfo->isRealType() == true)) {
            setProperty(EEADDR_REAL, true);
        }
    }

    VhValueEEAddress(const VhValueEEAddress &sourceAddr, TypeInfo *pTypeInfo)
    : VhTypedEEAddress (sourceAddr.m_flags & (~(EEADDR_NOT_DEL | EEADDR_NOT_DEL_ALLOC_SPACE)))
    , m_allocatedSize  (sourceAddr.m_allocatedSize)
    {
        if (m_allocatedSize == 0) {
            m_ptr               = NULL;
            m_partStat          = NULL;
            m_partStatInfoStart = 0;
        } else {
            m_ptr               = (char *) malloc(sourceAddr.m_allocatedSize);
            if (m_ptr == NULL) {
                HANDLE_MALLOC_FAIL_M_PTR
            } else {
                m_partStat          = (uint8T *) malloc(sourceAddr.getPartStatInfoSizeWithOffset());
                if (m_partStat == NULL) {
                    HANDLE_MALLOC_FAIL_M_PART_STAT
                } else {
                    memcpy(m_ptr, sourceAddr.m_ptr, m_allocatedSize);
                    memcpy(m_partStat, sourceAddr.m_partStat, sourceAddr.getPartStatInfoSizeWithOffset());
                    m_partStatInfoStart = sourceAddr.m_partStatInfoStart;
                }
            }
        }

        m_typeInfo = (pTypeInfo != NULL) ? ( pTypeInfo->merge(*sourceAddr.m_typeInfo))
                                         : (new TypeInfo(*sourceAddr.m_typeInfo));
        
        if ((m_typeInfo != NULL) && (m_typeInfo->isRealType() == true)) {
            setProperty(EEADDR_REAL, true);
        }
    }

    VhValueEEAddress(const VhValueEEAddress &sourceAddr, TypeInfo *pTypeInfo, uint32T flag)
    : VhTypedEEAddress    ((sourceAddr.m_flags & (~EEADDR_NOT_DEL)) | EEADDR_NOT_DEL_ALLOC_SPACE)
    , m_ptr               (sourceAddr.m_ptr)
    , m_allocatedSize     (sourceAddr.m_allocatedSize)
    , m_partStat          (sourceAddr.m_partStat)
    , m_partStatInfoStart (sourceAddr.m_partStatInfoStart)
    {
        if (sourceAddr.m_typeInfo != NULL) {
            m_typeInfo = (pTypeInfo != NULL) ? ( pTypeInfo->merge(*sourceAddr.m_typeInfo))
                                                      : (new TypeInfo(*sourceAddr.m_typeInfo));
        } else {
            JAG_ASSERT(&sourceAddr == &vhGi_nullEEAddressForAllocType);
            m_typeInfo = pTypeInfo;
        }
        if ((m_typeInfo != NULL) && (m_typeInfo->isRealType() == true)) {
            setProperty(EEADDR_REAL, true);
        }
    }

    virtual ~VhValueEEAddress() 
    {
        if((m_flags & EEADDR_NOT_DEL_ALLOC_SPACE) == 0) {
            if (m_ptr != NULL) {
                free(m_ptr);
                m_ptr      = NULL;
            }

            if (m_partStat != NULL) {
                free(m_partStat);
                m_partStat = NULL;
            }
        }
        TypeInfo::cleanTypeInfoTree(m_typeInfo);
    }

    VhValueEEAddress& operator = (const VhValueEEAddress &rhs)
    {
        m_flags             |= rhs.m_flags;
        m_ptr                = rhs.m_ptr;
        m_allocatedSize      = rhs.m_allocatedSize;
        m_partStat           = rhs.m_partStat;
        m_partStatInfoStart  = rhs.m_partStatInfoStart;
        m_typeInfo           = new TypeInfo(*rhs.m_typeInfo);
        return (*this);
    }

    void setFullyNonStatic ()
    {
        /* In case of LHS = RHS and RHS evaluates to NULL when RHS is a funcCall */
        /* which stops evaluation due to some non-static value during evaluation */
        /* then source's evaluated VhValueEEAddress is marked fully non-static        */

        for (uint64T i = 0; i < m_allocatedSize / 8; ++i) {
            setPartStatInfoDataType(i, nonStaticData);
        }
    }

    void copy_selected_target (const VhValueEEAddress &sourceAddr, uint64T sourceOffset)
    {
        JAG_ASSERT(m_ptr);
        uint64T targetSize = getAllocatedSize();

        JAG_ASSERT(sourceOffset + targetSize <= sourceAddr.getAllocatedSize());

        // Local copy of sourceAddr require to handle overlap cases.
        VhValueEEAddress *sourceAddrCopy = new VhValueEEAddress(sourceAddr);
        VhValueEEAddress  &sourceAddrCpy = *sourceAddrCopy;

        memcpy(m_ptr, sourceAddrCpy.m_ptr + sourceOffset, targetSize);

        for (uint64T i = 0; i < targetSize / 8; i++) {
            setPartStatInfoDataType(i, sourceAddrCpy.getPartStatInfoDataType(sourceOffset / 8 + i));
        }
        vha_deleteEEAddress(sourceAddrCopy);
    }

    EEerrCode checkSizeMatch(const VhValueEEAddress *rhsAddr, VhBase *lhsNode, VhExpr *rhsNode, bool bIsFormal, bool bReportError);

    void copy (const VhValueEEAddress &sourceAddr, const VhValueEEAddress *initAddr, VhBase *lhsNode, VhExpr *rhsNode,
                                                            bool bIsFormal, bool bReportError)
    {
        if ((sourceAddr.getProperty(EEADDR_NULL_STRING) == true)
         && (m_typeInfo != NULL)
         && (m_typeInfo->getTotalDimensionsInHier() == 1)
         && (m_typeInfo->m_indexConstraint->getConstraintsType() == unConstraintArrayType)) {
            setProperty(EEADDR_NULL_STRING, true);
            setProperty(EEADDR_NULL_RANGE, true);
        }

        // Revisit strategy for handling unconstrained RHS
        if (m_typeInfo == NULL) {
            setTypeInfo(sourceAddr.m_typeInfo);
        } else if (m_typeInfo->m_isConstrainedUptoThis == false) {
            //vha_deleteTypeInfo(m_typeInfo);
            TypeInfo *oldTypeInfo = m_typeInfo;
            m_typeInfo = m_typeInfo->merge(*sourceAddr.m_typeInfo);
            TypeInfo::cleanTypeInfoTree(oldTypeInfo);
        }

        //if (sourceAddr.getProperty(EEADDR_NULL_RANGE)) {
            //setProperty(EEADDR_NULL_RANGE, true);
        //}

        EEerrCode status = checkSizeMatch(&sourceAddr, lhsNode, rhsNode, bIsFormal, bReportError);
        if (status == EE_DONT_PROCEED_TO_COPY) {
            return;
        }
        
        JAG_ASSERT(m_typeInfo->m_isConstrainedUptoThis == true);
        
        if (m_ptr == NULL) {
            JAG_ASSERT(m_typeInfo != NULL);
            m_allocatedSize          = m_typeInfo->getSize();
            m_ptr                    = (char *) malloc(m_allocatedSize);
            m_partStatInfoStart      = 0;
            uint64T partStatInfoSize = getPartStatInfoSize();
            m_partStat               = (uint8T *) malloc(partStatInfoSize);
            memset(m_partStat, 0, partStatInfoSize);
        }

        uint64T sourceSize = std::min(sourceAddr.getAllocatedSize(), m_allocatedSize);
        
        bool isSReal = sourceAddr.getProperty(EEADDR_REAL);
        bool isTReal = this->getProperty(EEADDR_REAL);

        // Local copy of sourceAddr require to handle overlap cases like
        // a(2 downto 0) := a(3 downto 1)
        VhValueEEAddress *sourceAddrCopy = new VhValueEEAddress(sourceAddr);
        VhValueEEAddress &sourceAddrCpy  = *sourceAddrCopy;

        if (isSReal != isTReal) {
            if (isSReal == true) {
                for (uint64T i = 0; i < sourceSize / 8; i ++) {
                    setIntValue(i, (int64T)sourceAddrCpy.getRealValue(i));
                }
            } else {
                for (uint64T i = 0; i < sourceSize / 8; i ++) {
                    setRealValue(i, (real64T)sourceAddrCpy.getIntValue(i));
                }
            }
        } else {
            memcpy(m_ptr, sourceAddrCpy.m_ptr, sourceSize);
        }

        for (uint64T i = 0; i < sourceSize / 8; i ++) {
            if ((sourceAddrCpy.getPartStatInfoDataType(i) == openData) && (initAddr != NULL)) {
                setIntValue(i, initAddr->getIntValue(i));
                setPartStatInfoDataType(i, initAddr->getPartStatInfoDataType(i));
            } else {
                setPartStatInfoDataType(i, sourceAddrCpy.getPartStatInfoDataType(i));
            }
        }
        vha_deleteEEAddress(sourceAddrCopy);
        return;
    }

    bool markOpen(uint64T targetOffset, uint64T sourceSize)
    {
        for (uint64T i = 0; i < sourceSize / 8; i ++) {
            setPartStatInfoDataType(i + targetOffset / 8, openData);
        }
        return true;
    }

    bool arrAggrEleCopy (const VhValueEEAddress &sourceAddr, uint64T targetOffset, uint64T arrAggrEleSize, bool useSrcTypeInfoFrom1levelBelow)
    {
        uint64T sourceSize = std::min(sourceAddr.getAllocatedSize(), arrAggrEleSize);

        if ((m_allocatedSize - targetOffset) < sourceSize) {
            return false;
        }

        memcpy(m_ptr + targetOffset, sourceAddr.m_ptr, sourceSize);

        for (uint64T i = 0; i < sourceSize / 8; i ++) {
            setPartStatInfoDataType(i + targetOffset / 8, sourceAddr.getPartStatInfoDataType(i));
        }

        JAG_ASSERT(m_typeInfo != NULL);
        JAG_ASSERT(m_typeInfo->m_typeInfo != NULL);
        JAG_ASSERT(sourceAddr.m_typeInfo != NULL);
        if (useSrcTypeInfoFrom1levelBelow == true) {
            JAG_ASSERT((sourceAddr.m_typeInfo)->m_typeInfo != NULL);
        }

        // We might need to change merge to something like force merge for reassigning
        // aggregates of different size.
        if (m_typeInfo->m_typeInfo->getIsConstrainedUptoThis() == false) {
            TypeInfo *oldTypeInfo = m_typeInfo->m_typeInfo;
            m_typeInfo->m_typeInfo = m_typeInfo->m_typeInfo->merge((useSrcTypeInfoFrom1levelBelow == false)
                                                                        ? *(sourceAddr.m_typeInfo)
                                                                        : *((sourceAddr.m_typeInfo)->m_typeInfo));
            TypeInfo::cleanTypeInfoTree(oldTypeInfo);
            if (m_typeInfo->m_indexConstraint != NULL) {
                m_typeInfo->m_size = m_typeInfo->m_indexConstraint->getNumberOfElements() * m_typeInfo->m_typeInfo->getSize();
            }
        }
        return true;
    }

    uint64T recAggrEleCopy (const VhValueEEAddress &sourceAddr, uint64T targetOffset, RecordElemInfo *recInfo)
    {
        TypeInfo *elemTypeInfo = recInfo->getTypeInfo();
        if (elemTypeInfo->getIsConstrainedUptoThis() == false) {
            // If record element is access type, and the value is "NULL"
            // sourceAddr.getTypeInfo will be NULL.
            // Also in this merging constraints from value is meaningless
            if (sourceAddr.getTypeInfo() != NULL) {
                recInfo->setTypeInfo(elemTypeInfo->merge(*(sourceAddr.getTypeInfo())));
            }
            vha_deleteTypeInfo(elemTypeInfo);
            elemTypeInfo = recInfo->getTypeInfo();
        }
        JAG_ASSERT(elemTypeInfo->getIsConstrainedUptoThis() == true);
        uint64T recAggrEleSize = elemTypeInfo->getSize();
        uint64T sourceSize     = std::min(sourceAddr.getAllocatedSize(), recAggrEleSize);
        
        if ((m_allocatedSize - targetOffset) < sourceSize) {
            JAG_ASSERT(0);
        }

        memcpy(m_ptr + targetOffset, sourceAddr.m_ptr, sourceSize);

        for (uint64T i = 0; i < sourceSize / 8; i ++) {
            setPartStatInfoDataType(i + targetOffset / 8, sourceAddr.getPartStatInfoDataType(i));
        }

        return sourceSize;
    }

    void convertRealToInt(bool trucateVal)
    {
        if (getProperty(EEADDR_REAL) == true) {
            real64T  realval = getRealValue(0);
            int64T   intval  = (trucateVal == true) 
                               ? realval
                               : ((realval > 0.0) ? (realval + 0.5) : (realval - 0.5));
            setIntValue(0, intval);
            setProperty(EEADDR_REAL, false);
        }
    }

    bool integer_real_conv_copy (const VhValueEEAddress &sourceAddr, bool targetReal, bool srcReal)
    {
        if (m_typeInfo == NULL) {
            setTypeInfo(sourceAddr.m_typeInfo);
        } else if (m_typeInfo->m_isConstrainedUptoThis == false) {
            TypeInfo *oldTypeInfo = m_typeInfo;
            m_typeInfo            = m_typeInfo->merge(*(sourceAddr.m_typeInfo));
            TypeInfo::cleanTypeInfoTree(oldTypeInfo);
        }
        
        //if (m_indexConstraint && m_indexConstraint->isNullRange()) {
            //setProperty(EEADDR_NULL_RANGE, true);
        //}

        uint64T sourceSize  = sourceAddr.getAllocatedSize();
        if (sourceSize <= 0) {
            m_ptr               = NULL;
            m_allocatedSize     = 0;
            m_partStat          = NULL;
            m_partStatInfoStart = 0;
        } else {
            m_ptr                    = (char *) malloc(sourceSize);
            m_allocatedSize          = sourceSize;
            m_partStatInfoStart      = 0;
            uint64T partStatInfoSize = getPartStatInfoSize();
            m_partStat               = (uint8T *) malloc(partStatInfoSize);
            memset(m_partStat, 0, partStatInfoSize);

            if (targetReal != srcReal) {
                for (uint64T i = 0; i < sourceSize / 8; i ++) {
                    if (srcReal) {
                        // Real to Integer conversion
                        real64T  realval = sourceAddr.getRealValue(i);
                        int64T   intval  = (realval > 0.0) ? (realval + 0.5) : (realval - 0.5);
                        setIntValue(i, intval);
                    } else {
                        // Integer to Real conversion
                        int64T   intval  = sourceAddr.getIntValue(i);
                        real64T  realval = intval;
                        setRealValue(i, realval);
                    }
                }
            } else {
                memcpy(m_ptr, sourceAddr.m_ptr, sourceSize);
            }

            for (uint64T i = 0; i < sourceSize / 8; i ++) {
                setPartStatInfoDataType(i, sourceAddr.getPartStatInfoDataType(i));
            }
        }
        return true;
    }

    uint64T getAllocatedSize() const {
        return m_allocatedSize;
    }

    bool isMptrEightBytesAllZero(void) const {
        JAG_ASSERT(m_allocatedSize == 8);
        JAG_ASSERT(m_ptr != NULL);
        for (int i = 0; i < 8; i ++) {
            if (m_ptr[i] != 0) {
                return false;
            }
        }
        return true;
    }

    void truncateIntValue() const {
        if ((m_typeInfo != NULL) && (m_typeInfo->isIntegerType() == true)
         && (m_allocatedSize == sizeof(int64T)) 
         && (*m_partStat == CHAR_TYPE_FIRST_BIT_ONE)) {
#if 0
            int64T tempVal      = *((int64T*)(m_ptr));
            *((int64T*)(m_ptr)) = (int64T)((int32T)tempVal);
#endif
            // The aboove typecast for truncation may be fast than the
            // following code.
            if (((*(int64T*)(m_ptr)) & 0x0000000080000000LL) != 0LL) {
                (*(int64T*)(m_ptr)) |= 0xFFFFFFFF00000000LL;
            } else {
                (*(int64T*)(m_ptr)) &= 0x00000000FFFFFFFFLL;
            }
        }
    }

    int64T getIntValue(uint64T offset) const {
        return *(int64T*)(m_ptr + (offset << 3));
    }

    void setIntValue(uint64T offset, int64T value) {
        *(int64T*)(m_ptr + (offset << 3)) = value;
        setPartStatInfoDataType(offset, staticData);
    }

    real64T getRealValue(uint64T offset) const { 
        return *(real64T*)(m_ptr + (offset << 3)); 
    }

    void setRealValue(uint64T offset, real64T value) {
        *(real64T*)(m_ptr + (offset << 3)) = value;
        setPartStatInfoDataType(offset, staticData);
    }

    void getStdLogicStringValue(std::string &strVal) const {
        strVal.resize(m_allocatedSize / 8 + 1);
        strVal = StdLogicCharFromInt[this->getIntValue(0)];
        for (uint64T i = 1; i < m_allocatedSize / 8; i++) {
            strVal += StdLogicCharFromInt[this->getIntValue(i)];
        }
        strVal += '\0';
    }
    
    void getOneDimCharArrayStringValue(std::string &strVal, std::vector<char> intToCharArray) const {
        strVal.resize(m_allocatedSize / 8 + 1);
        strVal = intToCharArray[this->getIntValue(0)];
        for (uint64T i = 1; i < m_allocatedSize / 8; i++) {
            strVal += intToCharArray[this->getIntValue(i)];
        }
        strVal += '\0';
    }


    void getStringValue(std::string &strVal, bool isBitString) const {
        char offset = isBitString == true ? '0' : '\0';
        strVal.resize(m_allocatedSize / 8 + 1);
        strVal = (char)getIntValue(0) + offset;
        for (uint64T i = 1; i < m_allocatedSize / 8; i++) {
            strVal += (char)getIntValue(i) + offset;
        }
        strVal += '\0';
    }
    
    /*
    //written in order to support implicit to_hstring and to_ostring for std_logic_vector and std_ulogic_vector
    char getStdLogicCharValueFromBinary(std::string strVal) const {
        if(!strVal.compare("000") || !strVal.compare("0000")) {
            return '0';
        } else if (!strVal.compare("001") || !strVal.compare("0001")) {
            return '1';
        } else if (!strVal.compare("010") || !strVal.compare("0010")) {
            return '2';
        } else if (!strVal.compare("011") || !strVal.compare("0011")) {
            return '3';
        } else if (!strVal.compare("100") || !strVal.compare("0100")) {
            return '4';
        } else if (!strVal.compare("101") || !strVal.compare("0101")) {
            return '5';
        } else if (!strVal.compare("110") || !strVal.compare("0110")) {
            return '6';
        } else if (!strVal.compare("111") || !strVal.compare("0111")) {
            return '7';
        } else if (!strVal.compare("1000")) {
            return '8';
        } else if (!strVal.compare("1001")) {
            return '9';
        } else if (!strVal.compare("1010")) {
            return 'A';
        } else if (!strVal.compare("1011")) {
            return 'B';
        } else if (!strVal.compare("1100")) {
            return 'C';
        } else if (!strVal.compare("1101")) {
            return 'D';
        } else if (!strVal.compare("1110")) {
            return 'E';
        } else if (!strVal.compare("1111")) {
            return 'F';
        } else if (!strVal.compare("ZZZ") || !strVal.compare("ZZZZ")) {
            return 'Z';
        } else {
            return 'X';
        }
    }

    void  getStdLogicHexOrOctValue(std::string &strVal, bool isHex) const {
        uint64T base    = (isHex == true) ? 4 : 3;
        uint64T orgSize = m_allocatedSize >> 3;
        uint64T strSize = orgSize / base;

        if (orgSize % base != 0) {
            strSize ++;
        }

        strVal.resize(strSize + 1);
        int64T      padSize = strSize * base - 1;
        std::string tempStr = "";
        uint64T     strInd  = 0;
        for (int64T i = padSize; i >= 0 ; i -= base) {
            tempStr.resize(base);
            for (int64T j = 0; j < base ; j++) {
                if ((i - j) >= orgSize) {
                    tempStr[j] =  (StdLogicCharFromInt[this->getIntValue(0)] == 'Z' ? 'Z' : '0');
                } else {
                    tempStr[j] = StdLogicX01ZFromInt[this->getIntValue(orgSize - (i - j + 1))];
                }
            }
            strVal[strInd++] = getStdLogicCharValueFromBinary(tempStr);
        }
        strVal += '\0';
    }
    */

    void getHexOrOctValue(std::string &strVal, bool isHex) const {
        uint64T base    = (isHex == true) ? 4 : 3;
        uint64T strSize = m_allocatedSize >> 3;
        if (strSize % base == 0) {
            strSize /= base;
        } else {
            strSize = strSize / base + 1;
        }
        strVal.resize(strSize + 1);
        uint64T strPos = strSize - 1;
        uint64T j      = 0;
        uint64T value  = 0;
        for (uint64T i = (m_allocatedSize >> 3); i > 0; i--) {
            value |= (getIntValue(i - 1) << j);
            j++;
            if(j == base || i == 1) {
                j = 0;
                value = (value > 9) ? (value + 'A' - 10)
                                    : (value + '0');
                strVal[strPos--] = (char)value;
                value = 0;
            }
        }
        strVal += '\0';
    }

    bool getBoolValue() const
    {
        return ((getProperty(EEADDR_REAL) == true) 
                ? ((getRealValue(0) != 0.0) ? true : false)
                : ((getIntValue(0)  != 0LL) ? true : false)); 
    }

    void populateFromString(char *buf, bool reallocate)
    {
        int32T strLength = strlen(buf);
        if (reallocate == true) {
            m_ptr                    = (char*)malloc(strLength * 8);
            m_allocatedSize          = strLength * 8;
            uint64T partStatInfoSize = getPartStatInfoSize();
            m_partStat               = (uint8T *) malloc(partStatInfoSize);
            memset(m_partStat, 0, partStatInfoSize);
        }
        uint64T len = strLength;
        if ((len != 0) && (buf[len-1] == '\n')) {
            len--;
        }
        for (uint64T i = 0; i < len; i++) {
            int64T val = (int64T)buf[i]; 
            setIntValue(i, val);
        }
    }

    uint64T getPartStatInfoSizeWithOffset() const {
        uint64T noOfElems    =  (m_allocatedSize >> 3);
        noOfElems            += m_partStatInfoStart;
        uint64T partStatSize =  (noOfElems >> 3) + (((noOfElems & 0x7) == 0) ? 0 : 1);
        return partStatSize;
    }

    uint64T getPartStatInfoSize() const {
        uint64T noOfElems    = (m_allocatedSize >> 3);
        uint64T partStatSize = (noOfElems >> 3) + (((noOfElems & 0x7) == 0) ? 0 : 1);
        return partStatSize;
    }

    EEDataType getPartStatInfoDataType(uint64T offset) const {
        uint64T bitOffset  = (offset % 8) + m_partStatInfoStart;
        uint64T byteOffset = (offset / 8);
        if(bitOffset >= 8) {
            byteOffset ++;
            bitOffset -= 8;
        } 
        JAG_ASSERT((m_allocatedSize / 8) > offset);
        EEDataType retDataType = nonStaticData;
        if ((m_partStat[byteOffset] & (CHAR_TYPE_FIRST_BIT_ONE >> bitOffset)) != 0) {
            retDataType = staticData;
        } else if (getIntValue(offset) == OPEN_DATA_64_BIT) {
            retDataType = openData;
        } else {
            retDataType = nonStaticData;
        }
        return retDataType;
    }

    void setPartStatInfoDataType(uint64T offset, EEDataType dType) {
        uint64T bitOffset  = (offset % 8) + m_partStatInfoStart;
        uint64T byteOffset = (offset / 8);
        if(bitOffset >= 8) {
            byteOffset ++;
            bitOffset -= 8;
        }
        if (dType == staticData) {
            m_partStat[byteOffset] = m_partStat[byteOffset] | (CHAR_TYPE_FIRST_BIT_ONE >> bitOffset);
        } else if (dType == openData) {
            setIntValue(offset, OPEN_DATA_64_BIT);
            m_partStat[byteOffset] = m_partStat[byteOffset] & (~(CHAR_TYPE_FIRST_BIT_ONE >> bitOffset));
        } else {
            setIntValue(offset, ZERO_64_BIT);
            m_partStat[byteOffset] = m_partStat[byteOffset] & (~(CHAR_TYPE_FIRST_BIT_ONE >> bitOffset));
        }
    }

    bool isFullyStatic() const {
        if (getProperty(EEADDR_NULL_RANGE) == true) {
            return true;
        }
        if (m_partStat == NULL) {
            return false;
        } else if (m_allocatedSize == 0) { // One of the lower level dimension is NULL range.
            return true;
        }
        uint64T noOfElems  = (m_allocatedSize >> 3) + m_partStatInfoStart;
        uint64T valR3Bits  = noOfElems & 0x0000000000000007;
        uint64T noOfBytes  = (noOfElems >> 3) + ((valR3Bits == 0) ? 0 : 1);
        uint64T maxByteInd = noOfBytes - 1;

        if (maxByteInd == 0) {
            uint8T firstByte = (m_partStat[0] | maskFromL1[m_partStatInfoStart]) | maskFromR1[valR3Bits];
            if (firstByte != 0xFF) {
                return false;
            }
        } else {
            uint8T firstByte = m_partStat[0]          | maskFromL1[m_partStatInfoStart];
            uint8T lastByte  = m_partStat[maxByteInd] | maskFromR1[valR3Bits];
            if ((firstByte != 0xFF) || (lastByte != 0xFF)) {
                return false;
            }
            for (uint64T i = 1; i < maxByteInd; i++) {
                if (m_partStat[i] != 0xFF) {
                    return false;
                }
            }
        }
#if 0
        for (uint64T i = 0; i < m_allocatedSize / 8; i ++) {
            if (getPartStatInfoDataType(i) == nonStaticData) {
                return false;
            }
        }
#endif
        return true;
    }

    bool isSliceFullyStatic(uint64T offset, uint64T noOfEle) const {
        for (uint64T i = offset; i < (offset + noOfEle); i ++) {
            if (getPartStatInfoDataType(i) == nonStaticData) {
                return false;
            }
        }
        return true;
    }
    
    bool isFullyNonStatic() const {
        if (getProperty(EEADDR_NULL_STRING) || getProperty(EEADDR_NULL_RANGE)) {
            return false;
        }
        if (m_partStat == NULL) {
            return true;
        } else if (m_allocatedSize == 0) { // One of the lower level dimension is NULL range.
            return false;
        }
        assert(m_allocatedSize >= 8);
        uint64T noOfElems  = (m_allocatedSize >> 3) + m_partStatInfoStart;
        uint64T valR3Bits  = noOfElems & 0x0000000000000007;
        uint64T noOfBytes  = (noOfElems >> 3) + ((valR3Bits == 0) ? 0 : 1);
        uint64T maxByteInd = noOfBytes - 1;

        if (maxByteInd == 0) {
            uint8T firstByte = (m_partStat[0] & maskFromL0[m_partStatInfoStart]) & maskFromR0[valR3Bits];
            if (firstByte != 0x00) {
                return false;
            }
        } else {
            uint8T firstByte = m_partStat[0]          & maskFromL0[m_partStatInfoStart];
            uint8T lastByte  = m_partStat[maxByteInd] & maskFromR0[valR3Bits];
            if ((firstByte != 0x00) || (lastByte != 0x00)) {
                return false;
            }
            for (uint64T i = 1; i < maxByteInd; i++) {
                if (m_partStat[i] != 0x00) {
                    return false;
                }
            }
        }
#if 0
        for (uint64T i = 0; i < m_allocatedSize / 8; i ++) {
            if (getPartStatInfoDataType(i) == staticData) {
                return false;
            }
        }
#endif
        return true;
    }

    bool isEitherOperandNonStatic(const VhValueEEAddress &sourceAddr, uint64T lIndex, uint64T rIndex) const {
        EEDataType lDType = getPartStatInfoDataType(lIndex);
        EEDataType rDType = sourceAddr.getPartStatInfoDataType(rIndex);
        return ((((lDType | rDType) & nonStaticData) == 0) ? false : true);
    }

    bool interpretStaticStatus                (const VhValueEEAddress *pSrc1, const VhValueEEAddress *pSrc2, VhOpType opType);
    bool interpretStaticStatus_NonStatPriority(const VhValueEEAddress *pSrc1, const VhValueEEAddress *pSrc2, bool sameSize);
    bool interpretStaticStatus_opType         (const VhValueEEAddress *pSrc1, const VhValueEEAddress *pSrc2, VhOpType opType, bool sameSize);

    VhValueEEAddress* operator & (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator | (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator ^ (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator ! ()                                 const;
    VhValueEEAddress* reduction_and ()                              const;
    VhValueEEAddress* reduction_or  ()                              const;
    VhValueEEAddress* reduction_xor ()                              const;
    VhValueEEAddress* operator + (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator - (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator * (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator / (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator % (const VhValueEEAddress &rightOp)  const;
    VhValueEEAddress* operator + ()                                 const;
    VhValueEEAddress* operator - ()                                 const;
    VhValueEEAddress* abs ()                                        const;
    VhValueEEAddress* mod (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* exp (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* con (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* operator << (const VhValueEEAddress &rightOp) const;
    VhValueEEAddress* operator >> (const VhValueEEAddress &rightOp) const;
    VhValueEEAddress* sla (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* sra (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* rol (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* ror (const VhValueEEAddress &rightOp)         const;
    VhValueEEAddress* operator == (const VhValueEEAddress &rightOp) const;
    VhValueEEAddress* operator <  (const VhValueEEAddress &rightOp) const;
    VhValueEEAddress* operator <= (const VhValueEEAddress &rightOp) const;
    VhValueEEAddress* max ()                                        const;
    VhValueEEAddress* min ()                                        const;

    VhValueEEAddress* reduction_and_std_ulogic ()                   const;
    VhValueEEAddress* matchingRelOpStd_ulogic(const VhValueEEAddress &rightOp, int mapfunc[9][9]) const;

    void printAddressInDecimal() const {
        std::cout << "m_ptr Values in Decimal : \"";
        for (uint64T i = 0; i < m_allocatedSize / 8; i++) {
            if (getProperty(EEADDR_REAL) == true) {
                std::cout << *(real64T*)(m_ptr + (i * 8)) << "|";
            } else {
                std::cout << *(int64T*)(m_ptr + (i * 8)) << "|";
            }
        }
        std::cout << "\"" << std::endl;
        
        uint32T posCount = 0;
        std::cout << "m_partStat Values in Binary : \"";
        for (uint64T i = 0; i < m_allocatedSize / 8; i++) {
            if (getPartStatInfoDataType(i) == nonStaticData) {
                std::cout << "0";
            } else {
                std::cout << "1";
            }
            posCount ++;
            if ((posCount % 8) == 0) {
                std::cout << " ";
            }
        }
        std::cout << "\"" << std::endl;
    }

    virtual VhExpr *createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const;
    virtual char   *createVerilogExpression(const char *dutName) const;
    char *createLiterals(int64T &offset, TypeInfo *typeInfo, const char *dutName) const;
    std::string createVlogLiteralForOnedArray(int64T &offset, int64T noOfCurrEle, TypeInfo *elemTypeinfo, bool &status, const char *dutName) const;
    std::string createVlogLiteralForMultidArray(int64T &offset, ExprBoundConstraints *arrayCons, uint32T currentDim, TypeInfo *elemTypeinfo, bool &status, const char *dutName) const;
    char *createVlogLiteralForScalar(int64T &offset, VhBase *typeFrmTypeInfo, const char *dutName) const;

    VhExpr *createInternalAggregates(int64T &offset, TypeInfo *typeInfo, VhExpr *orgExpr, int memPlane) const;
    VhExpr *createAggregateFrom1DArray(int64T &offset, int64T width, TypeInfo *elemTypeinfo, VhExpr *orgExpr, int memPlane) const;
    VhExpr *createExprFromScalar(int64T &offset, VhBase *typeFrmTypeInfo, VhExpr *orgExpr, int memPlane) const;
    void   *getOneDimConstraint(VhExpr *orgExpr, int memPlane) const;

    virtual void browse() const;

    //static void printLiveEEAddresses();

    static VhValueEEAddress* copyAndDeleteAddr(VhValueEEAddress *&srcAddr, bool bForceCreateTypeInfo, TypeInfo *pTypeInfo) {
        VhValueEEAddress *copiedAddr = new VhValueEEAddress(*srcAddr, pTypeInfo);
        //if (srcAddr->getProperty(EEADDR_NOT_DEL) == false) {
            //srcAddr->m_typeInfo        = NULL;
            //srcAddr->m_indexConstraint = NULL;
        //}
        srcAddr = reinterpret_cast<VhValueEEAddress*>(vha_deleteEEAddress(srcAddr));
        return copiedAddr;
    }
};

class VhAllocEEAddress : public VhTypedEEAddress
{
    VhValueEEAddress *m_allocated;
    VhAllocEEAddress *m_aliasedAllocAddress;

public:
    VhAllocEEAddress(VhValueEEAddress *allocated)
    : VhTypedEEAddress (EEADDR_ALLOC)
    , m_allocated      (allocated)
    , m_aliasedAllocAddress(NULL)
    {
        setTypeInfo(allocated->getTypeInfo());
    }

    VhAllocEEAddress(VhValueEEAddress *allocated, VhAllocEEAddress *aliasedAllocAddress)
    : VhTypedEEAddress (EEADDR_ALLOC)
    , m_allocated      (allocated)
    , m_aliasedAllocAddress(aliasedAllocAddress)
    {
        setTypeInfo(allocated->getTypeInfo());
    }

    virtual ~VhAllocEEAddress() {}

    VhAllocEEAddress& operator = (const VhAllocEEAddress &rhs)
    {
        m_allocated  = rhs.m_allocated;
        if (m_aliasedAllocAddress != NULL) {
            //m_aliasedAllocAddress->setAllocatedAddr(m_allocated);
            *m_aliasedAllocAddress = *this;
        }
        //if (m_allocated == &vhGi_nullEEAddressForAllocType) {
        //    m_allocated  = rhs.m_allocated;
        //} else {
        //    *m_allocated = *rhs.m_allocated;
        //}
        return (*this);
    }

    VhValueEEAddress*  getAllocatedAddr() const {return m_allocated;}
    void setAllocatedAddr(VhValueEEAddress *allocated) { m_allocated = allocated; }

    virtual VhExpr* createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const {return orgExpr;}
    virtual char*   createVerilogExpression(const char *dutName) const {return NULL;}
    virtual void    browse() const;

private://undefined
    VhAllocEEAddress();
    VhAllocEEAddress(const VhAllocEEAddress &);
};

class VhExitNextEEAddress : public VhEEAddress 
{
    char *m_NextExitlabel;

public:
    VhExitNextEEAddress(char *pNextExitlabel, uint32T pIsNextExit)
    : VhEEAddress     (pIsNextExit)
    , m_NextExitlabel (pNextExitlabel)
    {
    }

    virtual ~VhExitNextEEAddress() {}

    char *getLabel() const { return m_NextExitlabel; }

    virtual VhExpr* createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const {assert(0); return NULL;}
    virtual char*   createVerilogExpression(const char *dutName) const {return NULL;}
    virtual void    browse() const;
private://undefined
    VhExitNextEEAddress();
    VhExitNextEEAddress& operator = (const VhExitNextEEAddress &);
    VhExitNextEEAddress(const VhExitNextEEAddress &);
};

class VhOpenEEAddress : public VhEEAddress
{
    uint64T m_openSize;

public:
    VhOpenEEAddress(uint64T openSize)
    : VhEEAddress (EEADDR_OPEN)
    , m_openSize  (openSize)
    {
    }

    virtual ~VhOpenEEAddress() {}
    uint64T  getOpenSize() const {return m_openSize;}

    virtual VhExpr* createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const {assert(0); return NULL;}
    virtual char*   createVerilogExpression(const char *dutName) const {return NULL;}
    virtual void    browse() const;

private://undefined
    VhOpenEEAddress();
    VhOpenEEAddress& operator = (const VhOpenEEAddress &);
    VhOpenEEAddress(const VhOpenEEAddress &);
};

class VhFileInfo
{
    friend class VhFileEEAddress;

    std::string m_fileName;
    FILE        *m_fp;
public:
    VhFileInfo(std::string pfileName, FILE *pFp)
    : m_fileName (pfileName)
    , m_fp       (pFp)
    {
    }
    ~VhFileInfo()
    {
        if (m_fp != NULL) {
            fclose(m_fp);
            m_fp = NULL;
        }
    }
};

class VhFileEEAddress : public VhEEAddress
{
    VhFileInfo  *m_fInfo;
    bool        m_copied;
    std::string m_fileDeclName;

public:
    VhFileEEAddress(const char *fdeclName)
    : VhEEAddress (EEADDR_FILE)
    , m_copied    (false)
    {
        m_fInfo        = new VhFileInfo("", NULL);
        m_fileDeclName = fdeclName;
    }

    VhFileEEAddress(const VhValueEEAddress &fileNameAddr, const char *fdeclName)
    : VhEEAddress(EEADDR_FILE)
    , m_copied(false)
    {
        std::string pfileName = "";
        fileNameAddr.getStringValue(pfileName, false);
        m_fInfo        = new VhFileInfo(pfileName, NULL);
        m_fileDeclName = fdeclName;
    }

    virtual ~VhFileEEAddress()
    {
        if ((m_fInfo->m_fp != NULL) && (m_copied == false)) {
            fclose(m_fInfo->m_fp);
            m_fInfo->m_fp = NULL;
        }
    }

    VhFileEEAddress& operator = (const VhFileEEAddress &rhs)
    {
        m_fInfo = rhs.m_fInfo;
        //if (m_fInfo->m_fp != NULL) {
            m_copied = true;
        //}
        return (*this);
    }

    int32T fileOpen(VhValueEEAddress *statAddr, const VhValueEEAddress *fileNameAddr, const VhValueEEAddress *modeAddr, std::string &fileName, EEerrCode &status)
    {
        if (fileNameAddr != NULL) {
            fileNameAddr->getStringValue(fileName, false);
        }
        if (m_fInfo->m_fp != NULL) {
            if (statAddr != NULL) {
                statAddr->setIntValue(0LL, 1LL);
            }
            //status = EE_FAILURE;
            return 2;
        } else {
            const char *mode = "r";
            if (modeAddr != NULL) {
                switch(modeAddr->getIntValue(0)) {
                    case 0:  mode = "r"; break;
                    case 1:  mode = "w"; break;
                    case 2:  mode = "a"; break;
                    default: break;
                }
            }
            fileName = vha_getEnvVarExpandedPathName(fileName);
            m_fInfo->m_fp = fopen(fileName.c_str(), mode);
            int32T errsv = errno;
            if (statAddr != NULL) {
                statAddr->setIntValue(0LL, m_fInfo->m_fp != NULL ? 0LL : 2LL);
            }

            vhGi_currentDataFileName = ((m_fInfo->m_fp != NULL) && (vhGi_currentDataFileName == NULL))? 
                vhw_strdup(const_cast<char*>(fileName.c_str())) : NULL;
            
            //status = m_fp != NULL ? EE_SUCCESS : EE_FAILURE;
            return (m_fInfo->m_fp == NULL) 
                   ? (errsv == EACCES) 
                     ? 5 
                     : ((errsv == ENOSPC) || (errsv == ENOENT))
                       ? 3
                       : 1
                   : 0;
        }
    }

    void   fileClose(EEerrCode &status)
    {
        if (m_fInfo->m_fp != NULL) {
            fclose(m_fInfo->m_fp);
            m_fInfo->m_fp       = NULL;
            m_fInfo->m_fileName = "";
            m_copied            = false;
        }
    }

    bool   getEOF(EEerrCode &status, int32T &category)
    {
        if (m_fInfo->m_fp != NULL) {
            char ch;
            if ((ch = fgetc(m_fInfo->m_fp)) == EOF) {
                return true;
            } else {
                ungetc(ch, m_fInfo->m_fp);
                return false;
            }
        } else {
            if (m_fInfo->m_fileName != "") {
                int32T category = fileOpen(NULL, NULL, NULL, m_fInfo->m_fileName, status);
                if (category == 0) {
                    return getEOF(status, category);
                }
            } else {
                category = 1;
                //status = EE_FAILURE;
                return false;
            }
        }
        return true;
    }

    void  readData(VhValueEEAddress *dataAddr, EEerrCode &status, VhValueEEAddress *lenAddr)
    {
        if (m_fInfo->m_fp != NULL) {
            if (dataAddr->getIsUnConstrained() == false) {
                uint64T size = dataAddr->getAllocatedSize();
                char    *buf = new char[size / 8 + 1];
                fgets(buf, size / 8 + 1, m_fInfo->m_fp);
                if (lenAddr != NULL) {
                    uint64T len = (uint64T)strlen(buf);
                    if ((len != 0) && (buf[len-1] == '\n')) {
                        len--;
                    }
                    lenAddr->setIntValue(0, len);
                }
                dataAddr->populateFromString(buf, false);
                delete [] buf;
            } else {
                static char buf[2048];
                fgets(buf, 2048, m_fInfo->m_fp);
                if (lenAddr != NULL) {
                    uint64T len = (uint64T)strlen(buf);
                    if ((len != 0) && (buf[len-1] == '\n')) {
                        len--;
                    }
                    lenAddr->setIntValue(0, len);
                }
                dataAddr->populateFromString(buf, true);
            }
        } else {
            if (m_fInfo->m_fileName != "") {
                int32T category = fileOpen(NULL, NULL, NULL, m_fInfo->m_fileName, status);
                if (category == 0) {
                    readData(dataAddr, status, lenAddr);
                }
            } else {
                status = EE_FAILURE;
            }
        }
    }

    const std::string& getFileDeclName() const {return m_fileDeclName;}

    virtual VhExpr* createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const {return orgExpr;}
    virtual char*   createVerilogExpression(const char *dutName) const {return NULL;}
    virtual void    browse() const;

private://undefined
    VhFileEEAddress();
    VhFileEEAddress(const VhFileEEAddress &);
};

template <typename T1, ExprConstraintsType consType>
class VhRangeEEAddress : public VhEEAddress
{
    ExprRangeConstraints<T1, consType>  _range;

public:
    VhRangeEEAddress(T1 left, T1 right, VhDirType dir)
    : VhEEAddress(EEADDR_RANGE)
    , _range(left, right, dir)
    {
        if (_range.isNullRange()) {
            setProperty(EEADDR_NULL_RANGE, true);
        }
    }

    virtual ~VhRangeEEAddress() {}

    void getRange(T1 &left, T1 &right, VhDirType &dir) const {
        _range.getRange(1, left, right, dir);
    }

    ExprRangeConstraints<T1, consType>* getCloneRangeCons() {
        return (new ExprRangeConstraints<T1, consType>(_range.left(1), _range.right(1), _range.getDirection(1)));
    }
    
    ExprBoundConstraints *generateExprBoundConstraints() {
        return (new ExprBoundConstraints(_range.left(1), _range.right(1), _range.getDirection(1)));
    }

    VhValueEEAddress *checkValueInRange(T1 valToCheck) {
        bool             result   = _range.checkValueInRange(valToCheck);
        VhValueEEAddress *retAddr = NULL;
        if (result == true) {
            retAddr = new VhValueEEAddress(ONE_64_BIT);
        } else {
            retAddr = new VhValueEEAddress(ZERO_64_BIT);
        }
        return retAddr;
    }

    virtual VhExpr *createExpression(VhExpr *orgExpr, bool createNstatAgg, bool retNullIfNS, int memPlane) const;
    virtual char*   createVerilogExpression(const char *dutName) const {return NULL;}
    virtual void    browse() const {
        VhEEAddress::browse();
        _range.browse();
    }

private://undefined
    VhRangeEEAddress();
    VhRangeEEAddress& operator = (const VhRangeEEAddress &);
    VhRangeEEAddress(const VhRangeEEAddress &);
};

template <typename ElementType> class  ExprOrTypeContainer 
{
    std::map<void*, ElementType*> m_eeAddress;

    void cleanup()
    {
        typename std::map<void *, ElementType*>::iterator it;
        for (it = m_eeAddress.begin(); it != m_eeAddress.end(); it++) {
            ElementType *tEeAddr = it->second;
            ElementType::DeleteObject(tEeAddr);
        }
        m_eeAddress.clear();
    }

public:
    ExprOrTypeContainer()  { m_eeAddress.clear(); }
    virtual ~ExprOrTypeContainer() { cleanup(); }

    void insert(void *pNode, ElementType *addr) {
        m_eeAddress[pNode] = addr;
    }

    void erase(void *pNode) {
        VhEEAddress *oldAddr = m_eeAddress[pNode];
        m_eeAddress.erase(pNode);
        vha_deleteEEAddress(oldAddr);
    }

    ElementType* findElement(void *pNode) {
        typename std::map<void *, ElementType*>::iterator it;
        it = m_eeAddress.find(pNode);
        if(it == m_eeAddress.end()) {
            return NULL;
        }
        return it->second;
    }

    virtual void browse()
    {
        typename std::map<void *, ElementType*>::iterator it;
        for (it = m_eeAddress.begin(); it != m_eeAddress.end(); it++) {
            ElementType *tEeAddr = it->second;
            tEeAddr->browse();
        }
    }
};

template <typename ElementType> class ExprOrTypeContainerStack 
{
    std::stack<ElementType *> m_eacStack;

    void cleanup() {
        while (!m_eacStack.empty()) {
            pop();
        }
    }

public:
    ExprOrTypeContainerStack()  { }
    virtual ~ExprOrTypeContainerStack() { cleanup(); }

    bool empty() const {
        return m_eacStack.empty();
    }

    uint64T size() const {
        return m_eacStack.size();
    }

    ElementType *top() const {
        return (m_eacStack.empty() ? NULL : m_eacStack.top());
    }

    void push(ElementType *pAddrContainer) {
        m_eacStack.push(pAddrContainer);
    }

    ElementType *pop_no_delete() {
        if (!m_eacStack.empty()) {
            ElementType *texprCont = m_eacStack.top();
            m_eacStack.pop();
            return texprCont;
        }
        return NULL;
    }

    void pop() {
        if (!m_eacStack.empty()) {
            ElementType *texprCont = m_eacStack.top();
            m_eacStack.pop();
            delete texprCont;
        }
    }
    virtual void browse()
    {
        ElementType *tele = top();
        if (tele != NULL) {
            tele->browse();
        }
    }
};

typedef ExprOrTypeContainer<VhEEAddress>                ExprAddrContainer;
typedef ExprOrTypeContainerStack<ExprAddrContainer>     ExprAddrContainerStack; 
typedef ExprOrTypeContainer<TypeInfo>                   ExprTypeInfoContainer;
typedef ExprOrTypeContainerStack<ExprTypeInfoContainer> TypeInfoContainerStack; 

extern VhDecl* resolveAliasExternal(VhBase *pDecl);
//#define JAG_NEW_REGR_REC_INST 1

template <typename stack_type, typename container_type, typename value_type>
class ContainerStackUtils
{
#ifdef JAG_NEW_REGR_REC_INST
public:
#endif
    stack_type m_stack;

public:
    // Constructor
    ContainerStackUtils() { }
 
    // Creates an empty container on top of the stack
    void createNewContainerOnStack() {
        m_stack.push(new container_type);
    }

    // Pushes a container on top of the stack
    void pushIntoStack(container_type *container) {
        m_stack.push(container);
    }

    // Insert pNode1, pNode2 pair on the container on the top of the stack
    void insertIntoTopOfContainerStack(void *pNode1, value_type *pNode2) {
        JAG_ASSERT(m_stack.empty() == false);
        container_type *pTop = m_stack.top();
        pTop->insert(pNode1, pNode2);
    }

    // finds EEAddress/TypeInfo of the pNode in the container on the to top the stack. 
    // Returns NULL if not found.
    value_type* findInTopOfContainerStack(void *pNode) {
        // Special handling is required which are not instantiated inside and entity/arch
        if (m_stack.empty() == true) {
            void *scope = ((VhBase *)pNode)->getScope();
            scope = resolveAliasExternal((VhBase *)scope);
            //JAG_ASSERT((((VhBase *)scope)->getObjectType() == VHPACKAGEDECL) || (((VhBase *)scope)->getObjectType() == VHPACKAGEBODY));
            createNewContainerOnStack();
        }
        JAG_ASSERT(m_stack.empty() == false);
        container_type *pTop = m_stack.top();
        return pTop->findElement(pNode);
    }

    // Removes an entry from the top of stack
    void deleteEntryFromTopOfStack(void *pDecl)
    {
        JAG_ASSERT(m_stack.empty() == false);
        container_type *pTop = m_stack.top();
        pTop->erase(pDecl);
    }

    // Pops the top of the stack. The entries in the container are not deleted
    void removeTopFromStack_no_delete() {
        JAG_ASSERT(m_stack.empty() == false);
        m_stack.pop_no_delete();
    }

    // Pops the top of the stack. The entries in the container are deleted
    // in the destructor of ExprOrTypeContainer.
    void removeTopFromStack() {
        JAG_ASSERT(m_stack.empty() == false);
        m_stack.pop();
    }

    // Returns the container on the top of the stack.
    container_type* getTopFromStack() {
        JAG_ASSERT(m_stack.empty() == false);
        return m_stack.top();
    }

    void cleanupEntriesFromContainerInTopOfStack()
    {
        JAG_ASSERT(m_stack.empty() == false);
        removeTopFromStack();
        createNewContainerOnStack();
    }
};

typedef ContainerStackUtils<ExprAddrContainerStack, ExprAddrContainer, VhEEAddress> EEAddrContainerStackUtils;
typedef ContainerStackUtils<TypeInfoContainerStack, ExprTypeInfoContainer, TypeInfo> TypeInfoContainerStackUtils;

template <typename T>
inline T eeaddress_cast(VhEEAddress *addr);

template <>
inline VhValueEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_VALUE) == true)) {
        return reinterpret_cast<VhValueEEAddress*>(addr);
    }
    return NULL;
}

template <>
inline VhAllocEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_ALLOC) == true)) {
        return reinterpret_cast<VhAllocEEAddress*>(addr);
    }
    return NULL;
}

template <>
inline VhExitNextEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_NEXT | EEADDR_EXIT) == true)) {
        return reinterpret_cast<VhExitNextEEAddress*>(addr);
    }
    return NULL;
}

template <>
inline VhOpenEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_OPEN) == true)) {
        return reinterpret_cast<VhOpenEEAddress*>(addr);
    }
    return NULL;
}

template <>
inline VhFileEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_FILE) == true)) {
        return reinterpret_cast<VhFileEEAddress*>(addr);
    }
    return NULL;
}

template <>
inline VhIntRangeEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_RANGE) == true)) {
        return reinterpret_cast<VhIntRangeEEAddress*>(addr);
    }
    return NULL;
}

template <>
inline VhRealRangeEEAddress* eeaddress_cast(VhEEAddress *addr)
{
    if ((addr != NULL) && (addr->getProperty(EEADDR_RANGE) == true)) {
        return reinterpret_cast<VhRealRangeEEAddress*>(addr);
    }
    return NULL;
}

#endif //__EXPR_EVAL_CLASSES_H__

