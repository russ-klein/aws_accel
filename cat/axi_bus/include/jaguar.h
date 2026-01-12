/*==========================================

 This file and related documentation are
 proprietary and confidential to Siemens
         Copyright Siemens 2021

===========================================*/

/************************************************************************
 
    File    : JAGUAR.h
 
    Purpose : Include file to be used for writing an application
              using JAGUAR PI routine set.

              This file lists out the various JPI pre-defined types
              and PI function prototypes.

    Enum Types :
            vhObjType
            vhOpType
            vhBoolean
            vhBaseType
            vhDirType
            vhPortType
            vhDisconnectSigKind
            vhAttribSpecEntityNameListType
            vhSignalKind
            vhSubProgType
            vhAttrbType
            vhTypeType
            vhStaticType
            vhEntClassType
            vhNameType
            vhIntFlagType
            vhPureType
            vhOpCategory
            vhResolutionMethod
            vhRegionType
 
************************************************************************/

#ifndef __JAGUAR_PI_H_
#define __JAGUAR_PI_H_       /* [[ start JPI include file */

#define JAG_FILE __FILE__

#include <stdarg.h>

#include <sys/types.h> /* to get definition of time_t */
#include <stdio.h> /* to get the definition of FILE * */
#include <stdint.h> /* to get the definition of FILE * */

//#ifdef WIN32
//#ifndef _NEW_IOSTREAM /* In Visual Studio 2005 iostream.h not exists */
//#include <iostream.h>
//    /* if not included - application can have linkage problems */
//#endif
//#endif



#include "ExprEvalClasses.h"

#if 0
enum VhObjType {
    VHBASE,
    VHNAMED,
    VHDESIGNUNIT,
    VHLIBRARYCLAUSE,
    VHUSECLAUSE,
    VHENTITY,
    VHCONFIGDECL,
    VHPACKAGEDECL,
    VHARCH,
    VHPACKAGEBODY,
    VHBLOCKCONFIG,
    VHCONFIGITEM,
    VHCOMPCONFIG,
    VHCOMPSPEC,
    VHCONFIGSPEC,
    VHENTITYASPECT,
    VHSEQSTMNT,
    VHWAIT,
    VHVARASGN,
    VHSIGASGN,
    VHWAVEFORM,
    VHWAVEFORMELEMENT,
    VHPROCEDURECALL,
    VHASSOCIATION,
    VHIF,
    VHCASE,
    VHCASEALTER,
    VHLOOP,
    VHWHILE,
    VHFOR,
    VHASSERT,
    VHREPORT,
    VHNEXT,
    VHEXIT,
    VHRETURN,
    VHNULL,
    VHCONCSTMNT,
    VHPROCESS,
    VHGENERIC,
    VHGENMAPASPECT,
    VHPORT,
    VHPORTMAPASPECT,
    VHCOMPONENT,
    VHSIGNAL,
    VHCONSTANT,
    VHVARIABLE,
    VHBLOCK,
    VHCONCPROCCALL,
    VHINSTANCE,
    VHCOMPINSTANCE,
    VHENTITYINSTANCE,
    VHCONCSIGASGN,
    VHCONDSIGASGN,
    VHCONDWAVEFORM,
    VHSELSIGASGN,
    VHSELWAVEFORM,
    VHGENERATE,
    VHIFGENERATE,
    VHFORGENERATE,
    VHEXPR,
    VHOPERATOR,
    VHUNARY,
    VHBINARY,
    VHFUNCCALL,
    VHQEXPR,
    VHTYPECONV,
    VHSELNAME,
    VHINDNAME,
    VHSLICENAME,
    VHATTRBNAME,
    VHPHYSICALLIT,
    VHDECLIT,
    VHBASELIT,
    VHENUMELEMENT,
    VHIDENUMLIT,
    VHSTRING,
    VHBITSTRING,
    VHCHARLIT,
    VHNULLLIT,
    VHSIMPLENAME,
    VHAGGREGATE,
    VHALLOCATOR,
    VHSUBPROGBODY,
    VHSUBPROGDECL,
    VHTYPEDECL,
    VHTYPEDEF,
    VHSUBTYPEDECL,
    VHRANGE,
    VHDISRANGE,
    VHSUBTYPEIND,
    VHCONSTRAINT,
    VHINDEXCONSTRAINT,
    VHRANGECONSTRAINT,
    VHENUMTYPE,
    VHINTTYPE,
    VHUNCONSARRAY,
    VHCONSARRAY,
    VHINDSUBTYPEDEF,
    VHOTHERS,
    VHALL,
    VHSIGNATURE,
    VHEXTERNAL,
    VHFORINDEX,
    VHOBJDECLARATION,
    VHLABEL,
    VHOBJECT,
    VHDEFAULTCONSTANT,
    VHDEFAULTVARIABLE,
    VHDEFAULTSIGNAL,
    VHCHAINDECL,
    VHATTRBDECL,
    VHATTRIBUTESPEC,
    VHENTITYDESIGNATOR,
    VHOPEN,
    VHSYMTABENTRY,
    VHSYMTAB,
    VHLIST,
    VHDECL,
    VHCONFINSTANCE,
    VHCONCSIGASGNSTMNT,
    VHGENERATESTMNT,
    VHCONCASSERT,
    VHDISCONNECTSPEC,
    VHBINDIND,
    VHENTITYENTITYASPECT,
    VHFILEDECL,
    VHALIASDECL,
    VHELEMENTASS,
    VHDELAYMECHANISM,
    VHPHYSICALTYPE,
    VHPRIMUNIT,
    VHSECUNIT,
    VHACCESSTYPE,
    VHRECORD,
    VHCONFIGENTITYASPECT,
    VHNAMEDLIST,
    VHNAMEDNODE,
    VHNAMEDITERATOR,
    VHLISTITERATOR,
    VHLISTNODE,
    VHELSIF,
    VHELEMENTDECL,
    VHFITS,
    VHSELECTEDNAME,
    VHSELDECL,
    VHOPENENTITYASPECT,
    VHATTRIBASSOC,
    VHOPSTRING,
    VHFLOATTYPE,
    VHGROUPTEMPLATE,
    VHENTITYCLASS,
    VHGROUPDECL,
    VHFILETYPE,
    VHFILENAME,
    VHSLICENODE,
    VHFUNCNODE,
    VHABSEXPR,
    VHARRAY,
    VHDIRECTIVE,
    VHDCSCRIPTDIRECTIVE,
    VHREGIONMARKER,
    VHDRIVER,
    VHNETBIT,
    VHPORTBIT,
    VHTERMBIT,
    VH_ERROR_OBJECT_TYPE,
    VHEXTERNALSIZE,
    VHCOMMENT,
    VHTERMINAL,
    VHUSERATTRIBUTE,
    VHATTRTABLE,
    VHUSERATTRIBNODE,
    VHTYPEOFATTRTABLE,
    VH_SCAN_ENTITY,
    VH_SCAN_PACKAGEDECL,
    VH_SCAN_CONTEXTDECL,
    VH_SCAN_CONFIGDECL,
    VH_SCAN_ARCH,
    VH_SCAN_PACKAGEBODY,
    VH64BITVALUE,
    VHPSLVERIFUNIT,
    VHPSLSTMT,
    VHPSLASSERT,
    VHPSLASSUME,
    VHPSLASSUMEGUARANTEE,
    VHPSLRESTRICT,
    VHPSLRESTRICTGUARANTEE,
    VHPSLCOVER,
    VHPSLFAIRNESS,
    VHPSLDECL,
    VHPSLCLOCKDECL,
    VHPSLNAMEDDECL,
    VHPSLPROPERTYDECL,
    VHPSLSEQUENCEDECL,
    VHPSLENDPOINTDECL,
    VHPSLPARAM,
    VHPSLFORALLINDEX,
    VHPSLEXPR,
    VHPSLPROPERTY,
    VHPSLFORALL,
    VHPSLPROPERTYOPERATOR,
    VHPSLUNARY,
    VHPSLBINARY,
    VHPSLTERNARY,
    VHPSLCLOCKED,
    VHPSLSEQUENCE,
    VHPSLSERE,
    VHPSLSERENORMAL,
    VHPSLSEREOPERATOR,
    VHPSLSERECLOCK,
    VHPSLSERECONCAT,
    VHPSLSEREREPEAT,
    VHPSLSERECOMPOSIT,
    VHPSLINSTANCE,
    VHPSLPROPERTYINSTANCE,
    VHPSLSEQUENCEINSTANCE,
    VHPSLENDPOINTINSTANCE,
    VHPSLUNION,
    VHPSLHDLEXPR,
    VHPSLBUILTINFUNCCALL,
    VHPSLOBJECT,
    VHPSLBINDUNITEXPR,
    VHPSLINFINITY,
    VHPSLPERCENTFOR,
    VHPSLPERCENTIF,
    VHPSLVUNITBOUNDUNIT,
    VHSHADOWOBJECT,
    VHSHADOWFITS,
    VHPSLPSLEXPR,
    VHPSLBOOLEAN,
    VHPSLPARAMETERIZEDPROPERTY,
    VHPSLPARAMETERIZEDSERE,
    VHPROTECTEDREGION,
    VHPROTECTEDDECL,
    VHPROTECTEDBODY,
    VH64BITREALVALUE,
    VHEXTERNALNAME,
    VHFORCEASGN,
    VHRELEASEASGN,
    VHELSIFGENERATE,
    VHELSEGENERATE,
    VHGENERATESTATEMENTBODY, 
    VHCASEGENERATE,
    VHCASEGENALTERNATIVE,
    VHSEQCONDSIGASGN,
    VHSEQSIGASGN,
    VHSEQSELSIGASGN,
    VHSEQWAVEFORMSIGASGN,
    VHSEQFORCERELSIGASGN,
    VHCONDEXPRESSION,
    VHSEQCONDFORCESIGASGN,
    VHSEQCONDSELEXPRSETBASE,
    VHSEQSELFORCESIGASGN,
    VHSELEXPRESSION,
    VHPROTECTDIRECTIVE,
    VHARRAYBASE,
    VHCONTEXTDECL,
    VHCONTEXTREF,
    VHMULTICONSTRAINTBASE,
    VHARRAYCONSTRAINT,
    VHVARASGNSTMNT,
    VHCONDVARASGN,
    VHSELVARASGN,
    VHRECINDXCONSTRAINTBASE,
    VHRECORDCONSTRAINT,
    VHRECORDELEMENTCONSTRAINT,
    VHSUBPROGINSTDECL,
    VHELSE,
    VHGENERICTYPE,
    VHPSLVUNITBINDIND,
    VHRESOLUTIONIND,
    VHSIMPLERESOLUTIONIND,
    VHARRAYRESOLUTIONIND,
    VHRECORDRESOLUTIONIND,
    VHRECELEMRESOLUTIONIND,
    VHPACKINSTDECL,
    VHGENSUBPROGDECL,
    VH_END_OF_OBJECT_TYPES
};

/*** External Name PathName Category ***/
enum VhExternalPathName {
        VH_ERROR_PATHNAME,
        VHPACKPATHNAME,
        VHABSPATHNAME,
        VHRELPATHNAME
};

enum VhAttrDensity {
        VH_DATA_MEMBER,
        VH_DENSE,
        VH_SPARSE
};

enum VhAttrDataType {
        VH_VOID_PTR,
        VH_CHAR_PTR,
        VH_INT_PTR,
        VH_FLOAT_PTR,
        VH_INT,
        VH_FLOAT,
        VH_SHORT_INT,
        VH_CHARACTER
};

enum VhOpType {
    VH_AND_OP,
    VH_OR_OP,
    VH_NAND_OP,
    VH_NOR_OP,
    VH_XOR_OP,
    VH_XNOR_OP,
    VH_EQ_OP,
    VH_NEQ_OP,
    VH_GTH_OP,
    VH_LTH_OP,
    VH_GEQ_OP,
    VH_LEQ_OP,
    VH_SLL_OP,
    VH_SRL_OP,
    VH_SLA_OP,
    VH_SRA_OP,
    VH_ROL_OP,
    VH_ROR_OP,
    VH_PLUS_OP,
    VH_MINUS_OP,
    VH_CONCAT_OP,
    VH_UPLUS_OP,
    VH_UMINUS_OP,
    VH_MULT_OP,
    VH_DIV_OP,
    VH_MOD_OP,
    VH_REM_OP,
    VH_EXPONENT_OP,
    VH_ABS_OP,
    VH_NOT_OP,
    VH_UAND_OP,
    VH_UOR_OP,
    VH_UNAND_OP,
    VH_UNOR_OP,
    VH_UXOR_OP,
    VH_UXNOR_OP,
    VH_COND_OP,    /* VHDL-2008 */ 
    VH_REL_EQ_OP,  /* VHDL-2008 support PR_14524 */
    VH_REL_NEQ_OP,
    VH_REL_GTH_OP,
    VH_REL_LTH_OP,
    VH_REL_GEQ_OP,
    VH_REL_LEQ_OP,
    VH_ERROR_OP,
    VH_SHL_OP, /*VH_SHL_OP and VH_SHR_OP are added for a support under mti-compatible flag ( PR-9001 )*/
    VH_SHR_OP
};

enum VhBoolean {
    VH_FALSE,
    VH_TRUE
};

/*
** Following enums have been defined in a specific order.
** The logic is depicted below :
** Say a is a node with start line/col :- (10,20)
**             &   with end   line/col :- (20,30)
** Now if we attach attributes on this node, it will be
** decompiled according to the sequence of enum types.
** Means, for any node
**    (o) Nodes with VH_EDIT_INSERT_BEFORE_LINE will be printed
**        first - because they will come just at the previous
**        line number of the current node.
**    (o) Nodes with VH_EDIT_INSERT_BEFORE      will be printed
**        next  - because they will come just at the previous
**        column number of the current node.
**    Similarly,
**    (o) Nodes with VH_EDIT_NODE_REPLACE will come next.
**    (o) Nodes with VH_EDIT_NODE_DELETE will come next.
**    (o) Nodes with VH_EDIT_INSERT_AFTER will come next.
**    (o) Nodes with VH_EDIT_INSERT_AFTER_LINE will come next.
**
** Before adding any nodes, follow this logic.
** ALSO VH_EDIT_INFO_ALWAYS_END must always be the last node.
*/

enum VhEditType {
    VH_EDIT_INSERT_BEFORE_LINE,
    VH_EDIT_INSERT_BEFORE,
    VH_EDIT_NODE_REPLACE,
    VH_EDIT_REPLACE_NAME,
    VH_EDIT_REPLACE_ENDLINE,
    VH_EDIT_NODE_DELETE,
    VH_EDIT_NODE_DELETE_LINE,
    VH_EDIT_NODE_DELETE_ENDLINE,
    VH_EDIT_INSERT_AFTER,
    VH_EDIT_INSERT_AFTER_LINE,
    VH_EDIT_INFO_ALWAYS_END
};

enum VhBackWardCompatibility {
    VH_NONLRM_LABEL,
    VH_DONT_CREATE_STR_FROM_AGG_IN_EVAL,
    VH_USE_OLDCLOCK_SCHEME_FOR_PROCESS,
    VH_SAVE_GENCONS_DURING_DIS_ELAB,
    VH_ALLOW_VHFUNCCALL_NODE_IN_ALIAS_DECL,
    VH_REPORT_LINENO_ONE_IN_WARNING_1141,
    VH_USE_OLD_DEFAULT_BINDING_SCHEME,
    VH_DISABLE_CONS_LOCALLY_EVAL_CHECK,
    VH_DONT_CREATE_FUNCNODE_FOR_SLICE_TYPECONV,
    VH_DISABLE_NONSTAT_FUNCCALL_SUPPORT_IN_PORT_MAP
};

enum VhBaseType {
    VH_BINARY,
    VH_UNSIGNED_BINARY,
    VH_SIGNED_BINARY,
    VH_OCTAL,
    VH_UNSIGNED_OCTAL,
    VH_SIGNED_OCTAL,
    VH_HEX,
    VH_UNSIGNED_HEX,
    VH_SIGNED_HEX,
    VH_DECIMAL,
    VH_ERROR_BASETYPE
};

enum VhDirType {
    VH_TO,
    VH_DOWNTO,
    VH_ERROR_DIR
};

enum VhPortType {
    VH_IN,
    VH_OUT,
    VH_INOUT,
    VH_BUFFER,
    VH_LINKAGE,
    VH_DEFAULT_IN,
    VH_NOT_PORT,
    VH_ERROR_PORTTYPE
};

enum VhDisconnectSigKind {
    VH_DISCONNECT_ALL,
    VH_DISCONNECT_OTHERS,
    VH_DISCONNECT_SIGNAL_LIST,
    VH_ERROR_DISCONNECT_SPEC
};

enum VhAttribSpecEntityNameListType {
    VH_ATTRIBSPEC_ALL,
    VH_ATTRIBSPEC_OTHERS,
    VH_ATTRIBSPEC_ENTITYDSG_LIST,
    VH_ERROR_ATTRIBSPEC_ENTNAME_LIST
};

enum VhSignalKind {
    VH_BUS,
    VH_REGISTER,
    VH_NOT_GUARDED,
    VH_ERROR_SIGTYPE
};

/* JAG-3025. New enum introduced to identify foreign attribute type */
enum VhForeignVhpiParseType {
    IGNORE_FOREIGN,
    VALID_FOREIGN,
    INVALID_FOREIGN
};

enum VhSubProgType {
    VH_PROCEDURE_SUBPG,
    VH_FUNCTION_SUBPG,
    VH_ERROR_SUBPROGTYPE
};

enum VhAttrbType {
    VH_LEFT,
    VH_RIGHT,
    VH_HIGH,
    VH_LOW,
    VH_LENGTH,
    VH_STRUCTURE,
    VH_BEHAVIOR,
    VH_VALUE,
    VH_POS,
    VH_VAL,
    VH_SUCC,
    VH_PRED,
    VH_LEFTOF,
    VH_RIGHTOF,
    VH_EVENT,
    VH_ACTIVE,
    VH_LAST_EVENT,
    VH_LAST_VALUE,
    VH_LAST_ACTIVE,
    VH_DELAYED,
    VH_STABLE,
    VH_QUIET,
    VH_TRANSACTION,
    VH_BASE,
    VH_RANGE,
    VH_REVERSE_RANGE,
    VH_ASCENDING,
    VH_IMAGE,
    VH_DRIVING,
    VH_DRIVING_VALUE,
    VH_SIMPLE_NAME,
    VH_INSTANCE_NAME,
    VH_PATH_NAME,
    VH_SUB_TYPE,
    VH_ELEMENT,
    VH_USER_DEF_ATTRB,
    VH_ERROR_ATTRBTYPE
};

enum VhTypeType {
    VH_INTEGER,
    VH_POSITIVE,
    VH_NATURAL,
    VH_BOOLEAN,
    VH_BIT_VECTOR,
    VH_BIT,
    VH_CHAR,
    VH_STRING,
    VH_BOOLEAN_VECTOR,
    VH_INTEGER_VECTOR,
    VH_REAL_VECTOR,
    VH_TIME_VECTOR,
    VH_USER_DEF_TYPE,
    VH_ERROR_TYPETYPE
};

enum VhStaticType {
    VH_LOCALLY_STATIC,
    VH_GLOBALLY_STATIC,
    VH_NOT_STATIC,
    VH_ERROR_STATIC_TYPE
};

enum VhEntClassType {
    VH_ENTITY,
    VH_ARCHITECTURE,
    VH_CONFIGURATION,
    VH_PROCEDURE,
    VH_FUNCTION,
    VH_PACKAGE,
    VH_TYPE,
    VH_SUBTYPE,
    VH_CONSTANT,
    VH_SIGNAL,
    VH_VARIABLE,
    VH_COMPONENT,
    VH_LABEL,
    VH_LITERAL,
    VH_UNITS,
    VH_GROUP,
    VH_FILE,
    VH_ERROR_ENTITY_CLASS
};

enum VhNameType {
    VH_PACKAGE_NAME,
    VH_PACKAGEBODY_NAME,
    VH_ENTITY_NAME,
    VH_LIBRARY_NAME,
    VH_RECORD_FIELD_NAME,
    VH_STATEMENT_LABEL_NAME,
    VH_ARCHITECTURE_NAME,
    VH_SUBPROGDECL_NAME,
    VH_PROCEDURE_NAME,
    VH_FUNCTION_NAME,
    VH_COMPONENT_NAME,
    VH_CONFIGURATION_NAME,
    VH_ALIASNAME,
    VH_SIMPLENAME,
    VH_VUNIT_NAME,
    VH_CONTEXT_NAME,
    VH_ERROR_NAME_TYPE
};

enum VhIntFlagType {
    VH_GEN,
    VH_PORT,
    VH_FUNC,
    VH_PROC,
    VH_RESET
};

enum VhPureType {
    VH_PURE,
    VH_IMPURE,
    VH_DEFAULT_PURE,
    VH_ERROR_PURETYPE
};

enum VhGenType {
    VH_SIMPLE,
    VH_UNINST,
    VH_GENMAP,
    VH_EXP_GENMAP
};

enum VhOpCategory {
    VH_LOGICAL,
    VH_RELATIONAL,
    VH_SHIFT,
    VH_ADDING,
    VH_SIGN,
    VH_MULTIPLYING,
    VH_MISC,
    VH_CONDITION,
    VH_ERROR_OPCATEGORY,
    VH_OTHERS /*VH_OTHERS is added for a support under mti-compatible flag ( PR-9001 ) */
};

enum VhResolutionMethod {
/* used for synopsys resolution method directive */
    VH_WIRED_OR,
    VH_WIRED_AND,
    VH_THREE_STATE,
    VH_NONE,
    VH_ERROR_RESOLUTIONMETHOD
};

enum RegionType {
    VH_TRANSLATION_OFF,
    VH_SYNTHESIS_OFF,
    VH_NO_REGION,
    VH_08_MODE_ON, /* JAG-3608 */
    VH_93_MODE_ON, /* JAG-3608 */
    VH_USER_DEFINED,
    VH_OVERRIDE_ON
};

enum VhRegionOrder {
    VH_DEFAULT,
    VH_LIFO,
    VH_FIFO,
    VH_RANDOM
};

enum VhMessgType {
    VH_INFO,
    VH_WARN,
    VH_SYNTH_WARN,
    VH_ERROR,
    VH_SYNTH_ERROR,
    VH_SYSERROR,
    VH_MESSG,
    VH_UNDISPLAY,
    VH_SHOWALWAYS
};

enum VhNameCaseType {
    VH_ASIS,
    VH_UPPER,
    VH_LOWER
};

enum VhDistinctNameType {
    VH_GENERIC_VALUE_PORT_SIZE,
    VH_GENERIC_NAME_VALUE_PORT_SIZE,
    VH_GENERIC_NAMEVALUE_PORT_SIZE,
    VH_OVERRIDDEN_GENERIC_NAME_VALUE_PORT_SIZE,
    VH_ERROR_DISTINCT_NAME_TYPE
};

enum VhPragmaObjType
{
    VH_DECL_OBJ,
    VH_STATEMENT_OBJ,
    VH_EXPR_OBJ,
    VH_SCOPE_OBJ,
    VH_SCOPE_DECL_STMT_OBJ,
    VH_UNKNOWN_OBJ
};

enum VhUserMetaCommentType
{
    VH_REGION_MARKER_INSIDE,
    VH_REGION_MARKER_PREV,
    VH_REGION_MARKER_NEXT,
    VH_REGION_MARKER_CURR_SCOPE,
    VH_DIRECTIVE_PREV,
    VH_DIRECTIVE_NEXT,
    VH_DIRECTIVE_CURR_SCOPE,
    VH_UNKNOWN_META_COMMENT_TYPE
};

enum VhPslVUnitType
{
    VHPSL_VUNIT,
    VHPSL_VPROP,
    VHPSL_VMODE,
    VHPSL_ERROR_VUNIT_TYPE
};

enum VhPslParamKind
{
    VHPSL_CONST,
    VHPSL_BOOLEAN,
    VHPSL_BIT,
    VHPSL_BITVECTOR,
    VHPSL_NUMERIC,
    VHPSL_STRING,
    VHPSL_HDLTYPE,
    VHPSL_PROPERTY,
    VHPSL_SEQUENCE,
    VHPSL_ERROR_PARAM_KIND
};

enum VhPslSereCompositType
{
    VHPSL_SERE_FUSION,
    VHPSL_SERE_OR,
    VHPSL_SERE_AND,
    VHPSL_SERE_AND_AND,
    VHPSL_SERE_WITHIN,
    VHPSL_SERE_ERROR_COMPOSIT_TYPE
};

enum VhPslSereRepeatType
{
    VHPSL_SERE_CONSEC_STAR,
    VHPSL_SERE_CONSEC_PLUS,
    VHPSL_SERE_NONCONSEC,
    VHPSL_SERE_GOTO,
    VHPSL_SERE_ERROR_REPEAT_TYPE
};

enum VhPslPropOpType
{
    VHPSL_CLOCKED,
    VHPSL_ABORT,
    VHPSL_NOT,
    VHPSL_OR,
    VHPSL_AND,
    VHPSL_IMPLICATION,
    VHPSL_IFF,
    VHPSL_NEXT,
    VHPSL_NEXT_STRONG,
    VHPSL_EVENTUALLY,
    VHPSL_ALWAYS,
    VHPSL_UNTIL,
    VHPSL_UNTIL_STRONG,
    VHPSL_UNTIL_INC,
    VHPSL_UNTIL_INC_STRONG,
    VHPSL_NEVER,
    VHPSL_BEFORE,
    VHPSL_BEFORE_STRONG,
    VHPSL_BEFORE_INC,
    VHPSL_BEFORE_INC_STRONG,
    VHPSL_NEXT_A,
    VHPSL_NEXT_A_STRONG,
    VHPSL_NEXT_E,
    VHPSL_NEXT_E_STRONG,
    VHPSL_NEXT_EVENT,
    VHPSL_NEXT_EVENT_STRONG,
    VHPSL_NEXT_EVENT_A,
    VHPSL_NEXT_EVENT_A_STRONG,
    VHPSL_NEXT_EVENT_E,
    VHPSL_NEXT_EVENT_E_STRONG,
    VHPSL_WITHIN,
    VHPSL_WITHIN_STRONG,
    VHPSL_WITHIN_INC,
    VHPSL_WITHIN_INC_STRONG,
    VHPSL_WHILENOT,
    VHPSL_WHILENOT_STRONG,
    VHPSL_WHILENOT_INC,
    VHPSL_WHILENOT_INC_STRONG,
    VHPSL_IMPLICATION_PROP,
    VHPSL_IMPLICATION_NON_OVERLAPPED,
    VHPSL_IMPLICATION_OVERLAPPED,
    VHPSL_ASYNC_ABORT,
    VHPSL_SYNC_ABORT,
    VHPSL_ERROR_PROPERTY_OP_TYPE
};

enum VhPslPslExprOpType
{
    VHPSL_IMPLICATION_OP,
    VHPSL_IFF_OP,
    VHPSL_ERROR_PSL_EXPR_OP
};

enum VhPslBuiltInFuncType
{
    VHPSL_ROSE_FUNC,
    VHPSL_FELL_FUNC,
    VHPSL_PREV_FUNC,
    VHPSL_NEXT_FUNC,
    VHPSL_STABLE_FUNC,
    VHPSL_ISUNKNOWN_FUNC,
    VHPSL_ONEHOT_FUNC,
    VHPSL_ONEHOT0_FUNC,
    VHPSL_COUNTONES_FUNC,
    VHPSL_ENDED_FUNC,
    VHPSL_NONDET_FUNC,
    VHPSL_NONDET_VECTOR_FUNC,
    VHPSL_ERROR_FUNC
};

/* JAG-3196 Start [ */

enum VhTypeDefConstraintType {
    VH_PARTIALLY_UNCONSTRAINT,
    VH_PARTIALLY_CONSTRAINT,
    VH_FULLY_UNCONSTRAINT,
    VH_FULLY_CONSTRAINT,
    VH_SCALOR_TYPEDEF,
    VH_UNKNOWN_TF_CONSTRAINT
};

/* ] End JAG-3196 */

/* JAG_2774 */
enum vhJAGPrNumber
{
    JAG_2774,
    JAG_2776,
    JAG_2783,
    JAG_2785,
    JAG_2787,
    JAG_2807,
    JAG_2053,
    JAG_1944,
    JAG_1934,
    JAG_2826,
    JAG_2055,
    JAG_1946,
    JAG_1954,
    JAG_1967,
    JAG_1968,
    JAG_1979,
    JAG_1980,
    JAG_1999,
    JAG_2012,
    JAG_2001,
    JAG_2871,
    JAG_2875,
    JAG_2004, 
    JAG_2000,
    JAG_2041,
    JAG_2043,
    JAG_2063,
    JAG_2921,
    JAG_2951,
    JVS_21,
    JAG_2955,
    JAG_3018,
    JAG_3081,
    JAG_3103,
    JAG_3146,
    JAG_3190,
    JAG_3202,
    JAG_3295,
    JAG_3159,
    JAG_3217,
    JAG_3306,
    JAG_3237,
    JAG_3344,
    JAG_3352,
    JAG_3360,
    JAG_3392,
    JAG_3404,
    JAG_3488
};

enum VhProtectKeyType
{
    VH_PROTECT_NONE,
    VH_PROTECT_BEGIN,
    VH_PROTECT_END,
    VH_PROTECT_BEGIN_PROTECTED,
    VH_PROTECT_END_PROTECTED,
    VH_PROTECT_VERSION,
    VH_PROTECT_AUTHOR,
    VH_PROTECT_AUTHOR_INFO,
    VH_PROTECT_ENCRYPT_AGENT,
    VH_PROTECT_ENCRYPT_AGENT_INFO,
    VH_PROTECT_KEY_KEYOWNER,
    VH_PROTECT_KEY_KEYNAME,
    VH_PROTECT_KEY_METHOD,
    VH_PROTECT_KEY_PUBLIC_KEY,
    VH_PROTECT_DATA_PUBLIC_KEY,
    VH_PROTECT_DATA_DECRYPT_KEY,
    VH_PROTECT_KEY_BLOCK,
    VH_PROTECT_DATA_KEYOWNER,
    VH_PROTECT_DATA_KEYNAME,
    VH_PROTECT_DATA_METHOD,
    VH_PROTECT_DATA_BLOCK,
    VH_PROTECT_DIGEST_KEYOWNER,
    VH_PROTECT_DIGEST_KEYNAME,
    VH_PROTECT_DIGEST_KEY_METHOD,
    VH_PROTECT_DIGEST_METHOD,
    VH_PROTECT_DIGEST_BLOCK,
    VH_PROTECT_DIGEST_PUBLIC_KEY,
    VH_PROTECT_DIGEST_DECRYPT_KEY,
    VH_PROTECT_ENCODING,
    VH_PROTECT_ENCODING_ENCTYPE,
    VH_PROTECT_ENCODING_LINE_LENGTH,
    VH_PROTECT_ENCODING_BYTES,
    VH_PROTECT_VIEWPORT,
    VH_PROTECT_VIEWPORT_OBJECT,
    VH_PROTECT_VIEWPORT_ACCESS,
    VH_PROTECT_DECRYPT_LICENSE,
    VH_PROTECT_RUNTIME_LICENSE,
    VH_PROTECT_LICENSE_LIBRARY,
    VH_PROTECT_LICENSE_ENTRY,
    VH_PROTECT_LICENSE_FEATURE,
    VH_PROTECT_LICENSE_EXIT,
    VH_PROTECT_LICENSE_MATCH,
    VH_PROTECT_COMMENT
};

#endif

/**** JAGUAR PI pre-defined types, #include "Const.h"  ****/

typedef void* vhNode;
typedef void* vhExpr;
typedef void* vhArray;
typedef void* vhEEAddress;

/*
//PR 14226
//We have changed all char * or String type to vhString
//type. vhString will behave as a const char * or char *
//depending upon one global macro
*/
typedef char* String; /* This should not be used in PI.h, kept since some of our internal function use this */
typedef const char* ConstString;

#ifdef WIN32
    #ifdef __GNUC_ 
        typedef long long vh64int;
        typedef long long IntegerVal;
        typedef unsigned long long vh64uint;
    #else
        typedef __int64 vh64int;
        typedef __int64 IntegerVal;
        typedef unsigned __int64 vh64uint;
    #endif
#else
    typedef long long vh64int;
    typedef long long IntegerVal;
    typedef unsigned long long vh64uint;
#endif

    typedef double vh64real ;

#ifdef _64BIT
#define VH_ULONG vh64uint
#else
#define VH_ULONG unsigned long
#endif

struct vhLinkStack {
    vhNode info;
    struct vhLinkStack* prev;
};
typedef struct vhLinkStack* vhStack;
typedef void* (* vh_callback_ptr) (void*);
typedef VhBoolean  (* vh_genericUpdationFuncPtr)(void);
typedef int (* vh_func_ptr) (void);
typedef int (* vh_config_callback_ptr) (void*, void **);
typedef enum VhBoolean (* vh_preParseFileHandlePtr)(const char*, int, void *);
typedef char* (* vh_protectedRegionHandler)(const char *);
typedef void (* vh_protectedTextDeallocator)(char *);
typedef char* (* vh_protectedTextComment)(int);
typedef enum VhBoolean (* vh_entity_exist_callback_ptr) (void*,void*,void*);
typedef char* (* vh_IPDumpEncryptionPrototype )(const char *, uint64_t *, int);
typedef char* (* vh_IPDumpDecryptionPrototype )(const char *, uint64_t *, void **);
typedef void (* vh_IPDumpDecryptedTextDeallocatorProto )(char *);
typedef void (* vh_IPDumpEncryptedTextDeallocatorProto )(char *);
typedef void (* vh_MVV_remove_replace_callback_ptr )(const char *, const char *,int );
typedef char* (* vh_einput_decrypt_ptr) (const vhEncryptionSourceInfo &);
typedef void (* vh_userFuncForPragma_ptr) (char *);
typedef void (* vh_callback_actionOnProfileBegin) (char *);
typedef void (* vh_callback_actionOnProfileEnd) (char *);
typedef void (*vh_einput_decrypt_buffer_deallocator_ptr)(char *, const vhEncryptionSourceInfo &);
typedef void (*vh_evaluateCallBackRtlc) (void *, int);
typedef int (*vh_evaluateCallBackRtlcToCheckForNonStaticExpr) (void *);
typedef void (*vh_CallbackForEndDU) (void *);
typedef void (*vh_CallbackForEndPackage) (char *);
typedef FILE* (* vh_callback_parse_fopen) (const char *); // dts0101130100
typedef int   (* vh_callback_parse_fclose)(const char *, FILE *&); // dts0101130100
typedef int (*vh_callback_actionOnProtectedRegion)(void*);
typedef vhNode (* vh_callback_createVhExprBasedOnValueFromGTable) (void *, const char *, void *);
typedef void (* vh_callback_mvvElaborateScopeWithGfile) (vhNode );
typedef void (* vh_callback_mvvUnElaborateScopeWithGfile) (vhNode );
typedef void (* vh_callback_mvvProcessingBeforeElabForConfig) (vhNode, vhNode, bool, bool );
typedef void (* vh_callback_mvvProcessingAfterUnElabForConfig) (vhNode );


/* 
  PR 8625 : distinguish between strings that can and cannot be freed.
  One new string type is introduced :
      vhString     : read-only strings (cannot be freed)
      vhUserString : strings that must be freed.
*/
#ifdef JAGUAR_CONST_STRING_TYPEDEFS
    typedef const char* vhString;
#else
    typedef char* vhString;
#endif
typedef char* vhUserString; /* string that must be freed */
typedef const char* vhConstString;



#ifdef __cplusplus    /* start _cplusplus [[ */

/* Type names to be used by PI application */
typedef VhObjType          vhObjType;
typedef VhDirType          vhDirType;
typedef VhBaseType         vhBaseType;
typedef VhPortType         vhPortType;
typedef VhDisconnectSigKind vhDisconnectSigKind;
typedef VhAttribSpecEntityNameListType vhAttribSpecEntityNameListType;
typedef VhSignalKind       vhSignalKind;
typedef VhOpType           vhOpType;
typedef VhPureType         vhPureType;
typedef VhSubProgType      vhSubProgType;
typedef VhForeignVhpiParseType      vhForeignVhpiParseType;
typedef VhBoolean          vhBoolean;
typedef VhEntClassType     vhEntityClass;
typedef VhNameType         vhNameType;
typedef VhStaticType       vhStaticType;
typedef VhOpCategory       vhOpCategory;
typedef VhAttrbType        vhAttrbType;
typedef VhTypeType         vhTypeType;
typedef RegionType         vhRegionType;
typedef VhResolutionMethod vhResolutionMethod;
typedef VhMessgType        vhMessageType;
typedef VhNameCaseType     vhNameCaseType;
typedef VhAttrDensity      vhAttrDensity;
typedef VhAttrDataType     vhAttrDataType;
typedef VhDistinctNameType vhDistinctNameType;
typedef VhRegionOrder      vhRegionOrder;
typedef VhUserMetaCommentType vhUserMetaCommentType;
typedef VhPragmaObjType      vhPragmaObjType;
typedef VhPslVUnitType        vhPslVUnitType;
typedef VhPslParamKind        vhPslParamKind;
typedef VhPslSereCompositType vhPslSereCompositType;
typedef VhPslSereRepeatType   vhPslSereRepeatType;
typedef VhPslPropOpType       vhPslPropOpType;
typedef VhPslPslExprOpType       vhPslPslExprOpType;
typedef VhPslBuiltInFuncType  vhPslBuiltInFuncType;
typedef VhTypeDefConstraintType  vhTypeDefConstraintType;
typedef VhBackWardCompatibility vhBackWardCompatibility;
typedef VhExternalPathName vhExternalPathName;
typedef VhProtectKeyType vhProtectKeyType;
typedef VhEditType          vhEditType;

#else

/* Type names to be used by PI application */
typedef enum VhObjType          vhObjType;
typedef enum VhDirType          vhDirType;
typedef enum VhBaseType         vhBaseType;
typedef enum VhPortType         vhPortType;
typedef enum VhDisconnectSigKind vhDisconnectSigKind;
typedef enum VhAttribSpecEntityNameListType vhAttribSpecEntityNameListType;
typedef enum VhSignalKind       vhSignalKind;
typedef enum VhOpType           vhOpType;
typedef enum VhPureType         vhPureType;
typedef enum VhSubProgType      vhSubProgType;
typedef enum VhForeignVhpiParseType      vhForeignVhpiParseType;
typedef enum VhBoolean          vhBoolean;
typedef enum VhEntClassType     vhEntityClass;
typedef enum VhNameType         vhNameType;
typedef enum VhStaticType        vhStaticType;
typedef enum VhOpCategory       vhOpCategory;
typedef enum VhAttrbType        vhAttrbType;
typedef enum VhTypeType         vhTypeType;
typedef enum RegionType         vhRegionType;
typedef enum VhResolutionMethod vhResolutionMethod;
typedef enum VhMessgType        vhMessageType;
typedef enum VhNameCaseType     vhNameCaseType;
typedef enum VhAttrDensity      vhAttrDensity;
typedef enum VhAttrDataType     vhAttrDataType;
typedef enum VhDistinctNameType vhDistinctNameType;
typedef enum VhRegionOrder      vhRegionOrder;
typedef enum VhUserMetaCommentType vhUserMetaCommentType;
typedef enum VhPragmaObjType      vhPragmaObjType;
typedef enum VhPslVUnitType        vhPslVUnitType;
typedef enum VhPslParamKind        vhPslParamKind;
typedef enum VhPslSereCompositType vhPslSereCompositType;
typedef enum VhPslSereRepeatType   vhPslSereRepeatType;
typedef enum VhPslPropOpType       vhPslPropOpType;
typedef enum VhPslPslExprOpType       vhPslPslExprOpType;
typedef enum VhPslBuiltInFuncType  vhPslBuiltInFuncType;
typedef enum VhTypeDefConstraintType  vhTypeDefConstraintType;
typedef enum VhBackWardCompatibility vhBackWardCompatibility;
typedef enum VhExternalPathName vhExternalPathName;
typedef enum VhProtectKeyType vhProtectKeyType;
typedef enum VhEditType          vhEditType;

#endif                           /* end _cplusplus ]] */


typedef vhBoolean (* vh_config_entity_callback_ptr) (char*, char *);

struct VhIterator {
    unsigned objType;
    vhNode  currPtr;    /* pointer to current Object in the List */
    vhNode  master;     /* pointer to the list whose iterator it is */
    vhBoolean isJagObj; /* whether list elements are Jaguar object */
};
#ifdef WIN32
typedef struct ::VhIterator *vhList;
typedef struct ::VhIterator _vh_list_;
#else
typedef struct VhIterator *vhList;
typedef struct VhIterator _vh_list_;
#endif

typedef void (* vh_callback_DumpFileHandler_ptr) (int, void *);
typedef void (* vh_callback_TextioMessageHandler_ptr) (vhString,int,int,vhString);
typedef char* (* vh_protectDecryptionRegionHandler )(vhList, const vhEncryptionSourceInfo &);
typedef char* (* vh_protectNextDecryptionRegionHandler )(const vhEncryptionSourceInfo &); //dts0101016605
typedef char* (* vh_protectEncryptionRegionHandler )(vhList);
typedef vhForeignVhpiParseType (* vh_callback_AttributeSpecHandler_ptr) (void *);
typedef void (* vh_callback_IfgenerateMessageHandler_ptr )(vhBoolean);

typedef struct {
    const char *sLibLogicalName;
    const char *sLibPhysicalPath;
    const char *sDumpFileName;
    unsigned int sDumpFileSize;    /*Bytes*/
    const char *sDepFileName;
    unsigned int sDepFileSize;    /*Bytes*/
    const char *sRecentFileName;
    unsigned int sRecentFileSize;    /*Bytes*/
} vhDumpFileInfoType;

/**** JAGUAR PI functions ****/

#ifdef _WIN32DLL
#define WIN_DLL_EXPORT __declspec(dllexport)
#else
#define WIN_DLL_EXPORT
#endif

#undef PROTO
#define PROTO(x) x
#ifdef __cplusplus        /* start ifdef _cplusplus [[ */
extern  "C" {
#endif                    /* end ifdef _cplusplus ]] */

WIN_DLL_EXPORT    int     vhInit PROTO((void));
WIN_DLL_EXPORT    vhBoolean vhIsInit PROTO((void));
WIN_DLL_EXPORT    vhString  VhGetObjectFileName PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    void VhInstanceElabObjSetMaster PROTO((vhNode elabObj, vhNode pArch, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetSourceFileName PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetAbsoluteSourceFileName PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhObjType VhGetObjType PROTO((vhNode obj, vhConstString file, int line));

WIN_DLL_EXPORT    int     VhGetLineNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetColumnNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetEndLineNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetEndColumnNumber PROTO((vhNode , vhConstString , int ));
WIN_DLL_EXPORT    int     VhGetIdentLineNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetIdentColumnNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetIdentEndLineNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetIdentEndColumnNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetBeginTokenLineNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetBeginTokenColNumber PROTO((vhNode , vhConstString , int ));
WIN_DLL_EXPORT    int     VhGetProcessSensListEndLineNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetProcessSensListEndColumnNumber PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    void     vhStoreAbsoluteFileName PROTO((vhBoolean flag));

    /* VHDL Design unit level functions */
WIN_DLL_EXPORT    vhString  VhGetLibName PROTO((vhNode designUnit, vhConstString file, int line));

WIN_DLL_EXPORT    vhString  VhGetInstanceAspectLibName PROTO((vhNode designUnit, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhOpenEntity PROTO((vhString libName, vhString entityName, vhConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhIsEntityBlackBox PROTO((vhNode entity, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhOpenArch PROTO((vhString libName, vhString entityName, vhString archName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenMRArch PROTO((vhString libName, vhString entityName, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhHandleOpenArch PROTO((vhNode entity, vhString archName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhHandleOpenMRArch PROTO((vhNode entity, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhOpenMRConfig PROTO((vhString libName, vhString entityName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhHandleOpenMRConfig PROTO((vhNode entity, vhConstString file, int line));

WIN_DLL_EXPORT    vhString  VhGetContextName PROTO((vhNode context, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetEntityName PROTO((vhNode entity, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetArchName PROTO((vhNode arch, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhOpenContextDecl PROTO((vhString libName, vhString contextName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenPackDecl PROTO((vhString libName, vhString packName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenPackBody PROTO((vhString libName, vhString packName, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPackName PROTO((vhNode packdecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenConfig PROTO((vhString libName, vhString confName, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetConfigName PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhIsEntityOpen PROTO((vhString libName, vhString entityName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsArchOpen PROTO((vhString libName, vhString entityName, vhString archName,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsConfigOpen PROTO((vhString libName, vhString configName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsPackDeclOpen PROTO((vhString libName, vhString packName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsPackBodyOpen PROTO((vhString libName, vhString packName, vhConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhHasEndLabel PROTO((vhNode designUnit, vhConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhCloseDesignUnit PROTO((vhNode designUnit, vhConstString file, int line));

WIN_DLL_EXPORT    int vhSaveDesignUnit PROTO((vhNode designUnit));

    /* Use clause and library clause PIs */
WIN_DLL_EXPORT    vhNode  VhGetLibraryClause PROTO((vhNode designUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetLibNameList PROTO((vhNode libClause, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetContextRef PROTO((vhNode designUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetUseClause PROTO((vhNode designUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSelNameList PROTO((vhNode useClause, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateCompSpec PROTO((vhNode compdef,vhList compins, vhConstString file, int line));

    /* Resolution Indication related APIS */

        /* Create APIs */

WIN_DLL_EXPORT    vhNode VhwCreateSimpleResolutionInd PROTO((vhNode A_ResFuncDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateArrayResolutionInd PROTO((vhNode A_ChildResInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRecordResolutionInd PROTO((vhList A_ChildResIndList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRecordElemResolutionInd PROTO((vhNode A_RecEleName, vhNode A_ResInd, vhConstString file, int line));

        /* Access APIs */

WIN_DLL_EXPORT    vhNode VhGetArrayEleSubtypeResFunc PROTO((vhNode A_ComplexResInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetRecordEleSubtypeResFunc PROTO((vhNode A_ComplexResInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhGetRecordElementNameExpr PROTO((vhNode A_RecEleInd, vhConstString file, int line));

    /* List and array utility functions */
WIN_DLL_EXPORT    vhNode  VhGetNextItem PROTO((vhList list, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetNextItemAddr PROTO((vhList list, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetListNodeNextPtr PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    void  VhwDeleteNodeAddrFromList PROTO((vhList,vhNode,vhNode,vhConstString,int));
WIN_DLL_EXPORT    void    VhSetListNodeInfoPtr PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetListNodeInfoPtr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhListGetCurrentNode PROTO((vhList list, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhListGetIthNode PROTO((vhList list,int position, vhBoolean fromStart, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhDeleteList PROTO((vhList list, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean vhDeleteVhList PROTO((vhList list));
WIN_DLL_EXPORT    vhBoolean vhFreeString   PROTO((vhString str));

WIN_DLL_EXPORT    int     VhListGetLength PROTO((vhList list, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhListRewind PROTO((vhList list, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetArraySize PROTO((vhArray array, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetNodeAt PROTO((int position, vhArray array, vhConstString file, int line));


    /* Routines for accessing subtype indication info */
WIN_DLL_EXPORT    vhBoolean VhIsResolved PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetResFunc PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetConstraint PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetExactConstraint PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetActualOfExternalSize PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhSetConstraint PROTO((vhNode subTypeInd,vhNode cons, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetOrgType PROTO((vhNode type, vhConstString file, int line)); /* returns VHEXTERNAL if found */
WIN_DLL_EXPORT    vhNode  VhGetType PROTO((vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetPrefixType PROTO((vhNode node, vhConstString file, int line));


    /* For accessing range information */
WIN_DLL_EXPORT    vhNode  VhGetRange PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhDirType VhGetRangeDir PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetLtOfRange PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetRtOfRange PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsRangeAttribute PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetRangeAttrib PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetDisRange PROTO((vhExpr, vhConstString file, int line));

    /* Routines for different type definitions */
WIN_DLL_EXPORT    vhString  VhGetTypeName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetTypeMark PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetRangeConstraint PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetRecEleName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetEleSubTypeInd PROTO((vhNode, vhConstString file, int line)); /* composite types */
WIN_DLL_EXPORT    vhList  VhGetRecEleList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetFileType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAccessType PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetTypeDef PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsIncType PROTO((vhNode typDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsGenType PROTO((vhNode typDecl, vhConstString file, int line));


    /* enum type definitions */
WIN_DLL_EXPORT    vhList  VhGetEnumEleList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    char    VhGetCharVal PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    char    VhGetCharEnumElement PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetCharLitValue PROTO((vhExpr expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsCharEnumLitElement PROTO((vhExpr expr, vhConstString file, int line));

WIN_DLL_EXPORT    vhString  VhGetIdEnumLit PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetIdEnumLitValue PROTO((vhNode enumLit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetEnumLit PROTO((vhExpr expr, vhConstString file, int line));

    /* different string literals access PI routines */
WIN_DLL_EXPORT    vhBaseType VhGetBaseSpec PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetStringVal PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetString PROTO((vhExpr expr, vhConstString file, int line));

    /* Based literals */
WIN_DLL_EXPORT    vhString  VhGetBaseLit PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    double    VhGetBaseLitValue PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    double    VhGetNumericValue PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    int       VhGetBase PROTO((vhExpr, vhConstString file, int line));

    /* physical literals */
WIN_DLL_EXPORT    vhExpr  VhGetAbsLit PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetUnitDecl PROTO((vhNode phyLit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetPrimUnitDecl PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSecUnitDeclList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetPhyLit PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetUnitName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetPhysicalUnitType PROTO((vhNode unit, vhConstString file, int line));
WIN_DLL_EXPORT    double  VhGetSecPrimUnitRatio PROTO((vhNode secUnit, vhConstString file, int line));
WIN_DLL_EXPORT    void    vhSetTime64BitPrecision PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vh64int vhGet64BitValue PROTO((vhNode expr));

WIN_DLL_EXPORT    void     vhSetReal64BitPrecision PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vh64real vhGetReal64BitValue PROTO((vhNode expr));


    /* decimal literals access functions */
WIN_DLL_EXPORT    vhString  VhGetIntPart PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetFloatPart PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetExponent PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    double  VhGetDecLitValue PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsDecLitPositive PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsReal PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsExponent PROTO((vhExpr, vhConstString file, int line));

    /* For constrained and unconstrained array types*/
WIN_DLL_EXPORT    vhList  VhGetIndexSubTypeDefList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetDiscRangeList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetArrayElementConstraint PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetArrayRootConstraint PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetIndexConstr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhTypeType VhGetTypeType PROTO((vhNode indSubTypeDef, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetNoOfDim PROTO((vhNode arrObj, vhConstString file, int line));
   /* For 2008 Generic Type */
WIN_DLL_EXPORT    vhList  VhGetDiscRangeListFromGenType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetGenMappedSubTypeInd PROTO((vhNode, vhConstString file, int line));


    /* PI routines relating to object declarations in VHDL */
WIN_DLL_EXPORT    vhString  VhGetSigName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetConstName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetVarName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetSubTypeInd PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetInitialValue PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetDefaultValueForObjDecl PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetDeferredConstantValue PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhSetInitialValue PROTO((vhNode decl, vhExpr value, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhSetLineNumber PROTO((vhNode obj, int value, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhSetEndLineNumber PROTO((vhNode obj, int value, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsAliasObject PROTO((vhNode object, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetActualForAlias PROTO((vhNode object, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAliasDeclFromObj PROTO((vhNode object, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetObjectAliasDeclaration PROTO((vhNode aliasDecl, vhNode scope, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsExplicitDecl PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT   vhBoolean VhIsAssociated PROTO((vhNode objDecl, vhConstString file, int line));
WIN_DLL_EXPORT   vhPortType VhGetPortType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT   vhDisconnectSigKind VhGetDisconnectSigKind PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT   vhAttribSpecEntityNameListType VhGetAttribSpecEntityNameListType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    int    VhGetBitStringLength PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhAllowDivideByZeroErrInAnalysis PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhAllowGloballyStaticInFormal PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhDisableShortCktCheckInEval PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhEnableEEAddressBasedExprEval PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhEvaluateFullyNonStaticExpr PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhSetUseEntNameAsBBNameForEntWithoutArch PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhEnableFeedThruFuncFlowCrtl PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhIgnoreSubprogVarDefaultInitialValue PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean   VhIsEEAddressBasedEval PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    char* VhCreateStdLogicStringFromEEAddr PROTO((vhEEAddress addr, vhConstString file, int line));
WIN_DLL_EXPORT    char VhGetStdLogicCharFromEEAddr PROTO((vhEEAddress addr, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhIgnore2008Keyword PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhEvalFuncHavingREAD PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhSetEvaluatingActualForInstConnections PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhSetCurrentRecursiveEntity PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhForceAggCreationForStr PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhAllowOpenInInputPortFormalSubElement PROTO((VhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean   VhIsSuppressibleError PROTO((int, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean   VhIs2008ModeOn PROTO((vhConstString file, int line));
//WIN_DLL_EXPORT    void   VhDisableVhdl0208Mix PROTO((VhBoolean flag, ConstString file, int line));
WIN_DLL_EXPORT    void   VhEnableVhdl0208Mix PROTO((VhBoolean flag, ConstString file, int line));
WIN_DLL_EXPORT    vhBoolean   VhIsVhdl0208MixEnabled PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGet2008Type PROTO((vhNode object));

    /* VHDL signals */
WIN_DLL_EXPORT    vhSignalKind VhGetSigKind PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetDisconnectTime PROTO((vhNode, vhConstString file, int line));

    /* VHDL constants */
WIN_DLL_EXPORT    vhBoolean VhIsDeferred PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsGeneric PROTO((vhNode constObj, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsSubProgInterfaceConstant PROTO((vhNode constObj, vhConstString file,
                                                        int line));
    /* VHDL variables */
WIN_DLL_EXPORT    vhBoolean VhIsSharedVar PROTO((vhNode, vhConstString file, int line));

    /* VHDL File declarations */
WIN_DLL_EXPORT    vhString  VhGetFileName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetFileOpenKind PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhPortType VhGetFileOpenMode_87 PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetFileLogicalName PROTO((vhNode, vhConstString file, int line));

    /* protected declaration PIs */
WIN_DLL_EXPORT    vhNode    VhGetProtectedDecl PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode    VhGetProtectedBody PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetProtectedName PROTO((vhNode, vhConstString file, int line));

    /* subprogram declaration PIs */
WIN_DLL_EXPORT    vhString  VhGetSubProgName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhSubProgType VhGetSubProgKind PROTO((vhNode, vhConstString file, int line)); /* procedure or function */
WIN_DLL_EXPORT    vhPureType VhGetPureType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsOverloadedOp PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetReturnType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetReturnList PROTO((vhNode subPBody, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetFlatParamList PROTO((vhNode subProgDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetBodyFlatParamList PROTO((vhNode subProgBody, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetSubProgSpec PROTO((vhNode, vhConstString file, int line)); /* From sub prog body */
WIN_DLL_EXPORT    vhNode  VhGetSubProgBody PROTO((vhNode subProgDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhSubProgHasUnconstrainedParam PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhSubprogUsesGlobals PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetGlobalsUsedInSubprog PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetConstantsUsedInSubprog PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetSubprogsUsedInSubprog PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsSubProgRecursive PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsSubProgOverloaded PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsScopeSynthesizable PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean vhArchIsSynthesizable  PROTO((vhNode));
WIN_DLL_EXPORT    void vhSetLibraryAndUseClause PROTO((vhString, vhString));
WIN_DLL_EXPORT    vhBoolean VhHasNonSynthProperty PROTO((vhNode scope, int flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhAllowOutsideReferenceInPureFunction(vhBoolean flag);


    /* Expression related PI routines */
WIN_DLL_EXPORT    vhNode  VhGetExpressionType PROTO((vhExpr expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhStaticType VhGetStaticType PROTO((vhExpr expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetStaticValue PROTO((vhExpr expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetExprSize PROTO((vhExpr expr, vhConstString file, int line));

WIN_DLL_EXPORT    vhOpCategory VhGetOpCat PROTO((vhNode op, vhConstString file, int line));
WIN_DLL_EXPORT    vhOpType  VhGetOpType PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetOperand PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetRightOperand PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetLeftOperand PROTO((vhExpr, vhConstString file, int line));

WIN_DLL_EXPORT    vhExpr  VhGetUninstSubProgName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetFuncCall PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetMaster PROTO((vhNode master, vhConstString file, int line)); /* return master of label/funccall/etc. */
WIN_DLL_EXPORT    vhBoolean  VhIsImplicitFuncCall PROTO((vhNode master, vhConstString file, int line));

WIN_DLL_EXPORT    vhList  VhGetAssocList PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhExpr  VhGetPrefix PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetExprList PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetEleAssocList PROTO((vhExpr, vhConstString file, int line)); /* aggregate expressions */
WIN_DLL_EXPORT    vhList  VhGetChoiceList PROTO((vhExpr, vhConstString file, int line));

WIN_DLL_EXPORT    vhExpr  VhGetAllocOperand PROTO((vhNode allocObj, vhConstString file, int line));

WIN_DLL_EXPORT    vhString   VhGetSimpleName PROTO((vhExpr simpleName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNameType VhGetSimpleNameType PROTO((vhExpr simpleName, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr   VhGetExpr PROTO((vhNode, vhConstString file, int line)); /* attribute name and element association */
WIN_DLL_EXPORT    void     vhForceSubElementAssociation PROTO((vhBoolean flag));

WIN_DLL_EXPORT    vhExpr  VhGetSelDeclExpr PROTO((vhNode, vhConstString file, int line));


    /* VHDL concurrent statements access PI routines */
    /* For blocks, component, entity */
WIN_DLL_EXPORT    vhNode  VhGetPortClause PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetGenericClause PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetFlatGenericList PROTO((vhNode genObj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetFlatSignalList PROTO((vhNode port, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsNamedAssociation PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhPortIsPurelyUnconstraint PROTO((vhNode port, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhPortIsGenericDependent PROTO((vhNode port, vhConstString file, int line));

    /* Routines for VHDL instances */
WIN_DLL_EXPORT    vhString  VhGetCompName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhArray VhGetPortMap PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhArray VhGetGenericMap PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetFormal PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetActual PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetSliceSize PROTO((vhNode ele, vhConstString file, int line));


    /* Only for VHDL block statements */
WIN_DLL_EXPORT    vhNode  VhGetPortMapAspect PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetGenericMapAspect PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsGuarded PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetGuardSignal PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetGuardExpr PROTO((vhNode, vhConstString file, int line));

    /* VHDL concurrent statements */
WIN_DLL_EXPORT    vhString  VhGetLabel PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetLabelObj PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsPostponed PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetConcStmtList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetConcStmtListLength PROTO((vhNode obj, vhConstString file, int line));

    /* VHDL for and for-generate statement routines */
WIN_DLL_EXPORT    vhNode  VhGetParamSpec PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetParamSpecId PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhExpr     VhCreateExprFromString PROTO((vhString str, vhNode scope,vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr     VhEvaluateConstantExpr PROTO((vhExpr expr,vhConstString file, int line));

    /* VHDL If, ElsIf and Else Generate statement routines */
WIN_DLL_EXPORT    vhString  VhGetAlternativeLabel PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetElsIfGenerateList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode    VhGetElseGenerateNode PROTO((vhNode, vhConstString file, int line));


    /* VHDL signal assignment statements */
WIN_DLL_EXPORT    vhExpr  VhGetTarget PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetWaveFrm PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhArray VhGetWaveFrmEleList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetValueExpr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetTimeExpr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetDelayMech PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsUnaffected PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsNullTransaction PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsTransport PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsReject PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsInertial PROTO((vhNode, vhConstString file, int line));

    /* Concurrent selected signal assignment */
WIN_DLL_EXPORT    vhExpr  VhGetSelExpr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSelWaveFrmList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetConditionalWaveFrmList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAssertion PROTO((vhNode, vhConstString file, int line)); /* concurrent assertion */
WIN_DLL_EXPORT    vhNode  VhGetProcedureCall PROTO((vhNode, vhConstString file, int line)); /* concurrent procedure call */


    /* VHDL sequential statements PI routines */

    /* Wait statement access PI routines */
WIN_DLL_EXPORT    vhList  VhGetSensitivityList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetConditionClause PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetTimeoutClause PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsSensitive PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetElseCondWaveFrm PROTO((vhNode condWaveFrm, vhConstString file, int line));

    /* Process and subprogram body routines */
WIN_DLL_EXPORT    vhBoolean VhHasWait PROTO((vhNode subProgBody, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhHasSignalAsgn PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhResolutionMethod VhGetResolutionMethod PROTO((vhNode subProgBody, vhConstString file, int line));

WIN_DLL_EXPORT    vhList  VhGetImplicitSensList PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetProcessClock PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetProcessClockExpr PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetProcessClockEdge PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetProcessAsyncSignalsList PROTO((vhNode process, vhConstString file, int line));

WIN_DLL_EXPORT    vhList  VhGetDriversList PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetDriverSignal PROTO((vhNode driver, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetDriverStatement PROTO((vhNode driver, vhConstString file, int line));
WIN_DLL_EXPORT    vhList vhGetReadOnlySignalsInProcess PROTO((vhNode process));
WIN_DLL_EXPORT    vhList vhGetWriteOnlySignalsInProcess PROTO((vhNode process));
WIN_DLL_EXPORT    vhList vhGetReadWriteSignalsInProcess PROTO((vhNode process));
WIN_DLL_EXPORT    vhList vhGetReadSignalsInExpr PROTO((vhNode expr));
WIN_DLL_EXPORT    vhList vhGetSetSignalList PROTO((vhNode process));
WIN_DLL_EXPORT    vhList vhGetResetSignalList PROTO((vhNode process));
WIN_DLL_EXPORT    vhList vhGetFloppedSignalList PROTO((vhNode process));
WIN_DLL_EXPORT    vhList vhGetFloppedSignalDriverList PROTO((vhNode sig));
WIN_DLL_EXPORT    vhList vhGetTriStateSignalList PROTO((vhNode node));
WIN_DLL_EXPORT    vhList VhGetReadOnlyObjectList PROTO((vhNode region, vhConstString file, int line));
WIN_DLL_EXPORT    vhList vhGetReadOnlyVariables PROTO((vhNode process));

WIN_DLL_EXPORT    vhExpr  VhGetSource PROTO((vhNode, vhConstString file, int line)); /* variable assignment */
WIN_DLL_EXPORT    vhExpr  VhGetProcedureName PROTO((vhNode, vhConstString file, int line)); /* procedure call object */

    /* IF statement routines */
WIN_DLL_EXPORT    vhExpr  VhGetCondition PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetThenSeqStmtList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetElsifSeqStmtList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetElsSeqStmtList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetElseStmtNode PROTO((vhNode, vhConstString file, int line));

    /* CASE statement routines */
WIN_DLL_EXPORT    vhExpr  VhGetCaseExpr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAlternativeList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsOthers PROTO((vhNode caseAlter, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetLoopStmt PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetEndLoopStmt PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetLoopLabel PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSeqStmtList PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhExpr  VhGetReportExpr PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetSeverityExpr PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetNextLabelObj PROTO((vhNode nextStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetExitLabelObj PROTO((vhNode exitStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAffectedLoop PROTO((vhNode obj, vhConstString file, int line));


    /* Attribute declarations and specifications */
WIN_DLL_EXPORT    vhString  VhGetAttribName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhEntityClass VhGetEntityClass PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetEntityDesigList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetEntityTag PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetAttribValue PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAttribDecl PROTO((vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetSignature PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetTypeMarkList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAttrb PROTO((vhNode attrbName, vhConstString file, int line));
WIN_DLL_EXPORT    vhAttrbType VhGetAttrbType PROTO((vhNode attrbName, vhConstString file, int line));

    /* Group declarations */
WIN_DLL_EXPORT    vhString  VhGetGroupTmplName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetEntityClassList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhEntityClass VhGetEntClassType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsBoxAtEnd PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetGroupDeclName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetGroupDeclTemplate PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetGroupConstituentList PROTO((vhNode, vhConstString file, int line));

    /* Disconnect specifications */
WIN_DLL_EXPORT    vhList  VhGetDisconnectSpecSignalList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetDisconnectSpecTime PROTO((vhNode, vhConstString file, int line));

    /* Alias declarations */
WIN_DLL_EXPORT    vhString  VhGetAliasName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAliasDesig PROTO((vhNode, vhConstString file, int line));

    /* Component specification and configuration */
WIN_DLL_EXPORT    vhList  VhGetInstList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetCompSpec PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetBindInd PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetBlockConfig PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhGetBlockSpec PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    int  VhGetBlockConfigUniqueId PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetEntityAspect PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsArchNameImplicitMRA PROTO((vhNode entAspect, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean vhIsImplicitMRAArch PROTO((vhString lib, vhString ent, vhString arch));
WIN_DLL_EXPORT    vhList  VhGetConfigItemList PROTO((vhNode, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetCompDecl PROTO((vhNode comp, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetConfigSpec PROTO((vhNode compInst, vhConstString file, int line));
WIN_DLL_EXPORT    void    vhSearchEntityInLib PROTO((vhString libName));
WIN_DLL_EXPORT    vhNode  VhGetDefaultBinding PROTO((vhNode compInst, vhConstString file, int line));

WIN_DLL_EXPORT    vhList  VhGetPslVUnitBindIndList PROTO((vhNode A_CompInst, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetPslVUnitBindIndExprList PROTO((vhNode A_ConfSpec, vhConstString file, int line));

    /* Miscellaneous utility PI routines */
WIN_DLL_EXPORT    void    VhSetAttr PROTO((vhNode object, vhNode value, vhString name, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetAttr PROTO((vhNode object, vhString name, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhDumpUserAttribute PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode    VhGetDUEncryptionInfo PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhStoreUserComponentName PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    void    VhStoreElseLineNoOfIfStmnt PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetElseLineNoOfIfStmnt PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhStoreComponentFormalList PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean    VhIsCompiledWith87Mode PROTO((vhNode ptr, vhConstString file, int line));
WIN_DLL_EXPORT    int    VhGetVHDLCompilationMode PROTO((vhNode ptr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhAttributeCreate PROTO((vhString name, vhObjType attachmentObject, vhAttrDensity density, vhAttrDataType dataType, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhNodeAttachAttribute PROTO((vhNode obj, vhNode attr, void* value, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhNodeGetAttributeValue PROTO((vhNode obj, vhNode attr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean vhIsJaguarDumpDirectory PROTO((vhString dirName));
WIN_DLL_EXPORT    vhNode  vhCreateAttributeTable PROTO((void));
WIN_DLL_EXPORT    vhNode  vhGetCurrentAttributeTable PROTO((void));
WIN_DLL_EXPORT    void    vhSetCurrentAttributeTable PROTO((vhNode attrTab));
WIN_DLL_EXPORT    void    vhDeleteAttributeTable PROTO((vhNode attrTab));
WIN_DLL_EXPORT    void    vhResetUserAttributes PROTO((void));
WIN_DLL_EXPORT    void    vhUpdateDepInfoOfDuList PROTO((vhList));
WIN_DLL_EXPORT    void    vhDeleteAnalyzedDuList PROTO((vhList));
WIN_DLL_EXPORT    int     vhIsUnitHasAnyOpenDependentUnit PROTO((vhNode));
WIN_DLL_EXPORT    vhNode  VhLookUpInSymTab PROTO((vhNode scope, vhString name, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhStopAtVhExternalNode PROTO((vhBoolean stop, vhConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhIsA PROTO((vhNode, vhObjType, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetObjTypeName PROTO((vhObjType obTyp, vhConstString file, int line));
WIN_DLL_EXPORT    vhObjType VhGetBaseObjType PROTO((vhObjType obTyp, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetDerivedTypes PROTO((vhObjType obTyp, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetScope PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetMasterScope PROTO((vhNode obj, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  VhGetActualObj PROTO((vhNode extrn, vhConstString file, int line));
WIN_DLL_EXPORT    vhObjType VhGetActualObjType PROTO((vhNode extrn, vhConstString file, int line));
                    /* return actual type of VHEXTERNAL */
WIN_DLL_EXPORT    vhObjType VhGetActualObjScopeType PROTO((vhNode extrn, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetActualObjScopeName PROTO((vhNode extrn, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPrimUnitName PROTO((vhNode extrn, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode    VhGetSynthesizedResFunction PROTO((vhNode funcDecl, int consLen, vhConstString file, int line));
WIN_DLL_EXPORT    void        vhUseSubProgIDMap PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhEnableAmgamCompatibilty PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhEnableSokiCompatibilty PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhEnableEscapedLibInJaguarc PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vhBoolean   vhEnableSimulatorDefBindFlow PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhEnableRelaxOption PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        VhSimulatorCompatible PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhVcsCompatible PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhAllowGenerateStmtWithoutBeginKeyword PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhAllowDefactoStdSyntax PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhMentorCompatible PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        vhSetNCCompatible PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhSetLowerCasePslEmbdMarker PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhSetRelaxedFileDeclCheck PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhCheckSubProgBody PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        VhStoreInitialValues PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        vhDisableSrcEvalInPropagateValue PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void        vhEnableLocallyStaticExprEval PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vhBoolean   VhPropagateValue     PROTO((vhExpr target, vhExpr source,vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean   VhPropagateValueVer2 PROTO((vhExpr target, vhExpr source,vhConstString file, int line));
WIN_DLL_EXPORT    void        VhReturnPartialStaticValue PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode      VhwCreateNonStaticValue PROTO((vhNode subTypeInd, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhDetectGatedClock PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhSupportEnumTypeGeneric PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void        VhLibraryPathCompatible PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhString    VhGetVersionInfo  PROTO((vhConstString file, int line));
WIN_DLL_EXPORT       vhNode    VhDetectClock PROTO((vhExpr cond, int* edge, vhConstString file, int line));
WIN_DLL_EXPORT       vhNode    VhDetectClockInWaitStmt PROTO((vhExpr cond, int* edge, vhConstString file, int line));
WIN_DLL_EXPORT    void        vhExitOnIllegalOption PROTO((vhBoolean flag)); 
WIN_DLL_EXPORT    void        vhEnableDumpCmdLineAttributes PROTO((vhBoolean flag)); 
WIN_DLL_EXPORT    vhList      vhGetDUCmdLineAttributes PROTO((vhString libName, vhString unitName, vhString primary)); 

WIN_DLL_EXPORT    vhBoolean   VhGetStatusOfEvalParialStatic   PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean   VhGetStatusOfIsPartValSupported PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhNode      VhResolveAliasExternal          PROTO((vhNode pDecl, vhConstString file, int line));
WIN_DLL_EXPORT    int         VhPrintAllGlobals               PROTO((char *outFileName, vhConstString file, int line));
  /* Functions to access the VHDL design library information */
WIN_DLL_EXPORT    vhList  VhGetAllPrimaryUnits PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAllSecondaryUnits PROTO((vhString libName, vhString primUnitName, vhConstString file, int line));

WIN_DLL_EXPORT    vhList  VhGetAllEntities PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAllPackages PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAllConfigurations PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAllContextDeclarations PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAllArchitectures PROTO((vhString libName, vhString entityName, vhConstString file, int line));

WIN_DLL_EXPORT    vhList  VhGetAllLibraries PROTO((vhConstString file, int line));
    /* Return list of visible logical library names */

WIN_DLL_EXPORT    vhString  VhGetLibPathName PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPhysicalPathFromLogicalName PROTO((vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  vhGetLogicalWorkLibrary PROTO((void));
WIN_DLL_EXPORT    vhString  vhGetApplicationDumpVersion PROTO((void));
WIN_DLL_EXPORT    void      vhEvaluateImpureFunction PROTO((vhBoolean flag)); 
WIN_DLL_EXPORT    void      vhSkipForeignSubProgramEvaluation PROTO((vhBoolean flag)); 
WIN_DLL_EXPORT    void      vhSkipBuiltInSubProgramEvaluation PROTO((vhBoolean flag)); 
WIN_DLL_EXPORT    void      vhSupportMutualRecursion PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void      vhForceDumpRecompilation PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vhUserString  vhGetDumpVersion(vhString libName, vhString primName, vhString secondaryName);

    /* Get physical path for library logical name */

WIN_DLL_EXPORT    vhBoolean VhIsUnitVerifUnit PROTO((vhString libName, vhString primUnitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsUnitEntity PROTO((vhString libName, vhString primUnitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsUnitPackage PROTO((vhString libName, vhString primUnitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsUnitConfiguration PROTO((vhString libName, vhString primUnitName, vhConstString file, int line));


    /* PI routines for declarative region */
WIN_DLL_EXPORT    vhList    VhGetDeclItemList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    int       VhGetDeclListLength PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetName PROTO((vhNode obj, vhConstString file, int line)); /* Get the name of a VHDECL object */
WIN_DLL_EXPORT    vhList    VhGetInterfaceList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetNonFlatInterfaceList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetExternalNameList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetPathNameListOfExtName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhGetUsedAsExtNameListInCurrScope PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhResolveExternalName PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhSetActualOfExtName PROTO((vhNode, vhNode, bool, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhSetInstanceMaster PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode    VhGetInstanceMaster PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExternalPathName   VhGetExtNamePathNameCategory PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhExtNameAddNodeToObjectList PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList    VhExtNameGetObjectList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    unsigned  VhExtNameGetUpwardRefCount PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhExtNameSetActualInMixed PROTO((vhNode, vhBoolean, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhExtNameGetActualInMixed PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhObjType VhGetExtNameClassType PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetExtNameObjName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhExtNamePopulateTypeInfo PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhSetExtNameStaticValue PROTO((vhNode, vhExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhGetExtNameElabOnly PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhSetExtNameLastEntityName PROTO((vhNode, vhString, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhSetExtNameParentNetName PROTO((vhNode, vhString, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetExtNameLastEntityName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetExtNameParentNetName PROTO((vhNode, vhConstString file, int line));

    /* Routines to access lists of different types */
WIN_DLL_EXPORT    vhList  vhGetListOfObjectsByTypeInScope PROTO((vhNode scope, vhObjType objectType));
WIN_DLL_EXPORT    int  vhGetNumberOfObjectsByTypeInList PROTO((vhList objList, vhObjType objectType));


WIN_DLL_EXPORT    vhList  VhGetSubProgDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSubProgBodyList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetFunctionDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetProcedureDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetTypeDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSubTypeDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetConfigSpecList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetCompDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSignalDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetConstDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetVarDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAttrbDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetAttrbSpecList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetEnumDeclList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetComponentDeclList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetCompInstList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetContextItemList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetContextDeclItemList PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetActiveLowSignalList PROTO((vhNode arch, vhConstString file, int line));


    /* Routines to return the names corresponding to the different enum type values */
WIN_DLL_EXPORT    vhString  VhGetOpTypeName PROTO((vhOpType opTyp, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetBooleanName PROTO((vhBoolean babool, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetBaseTypeName PROTO((vhBaseType baseTyp, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetDirTypeName PROTO((vhDirType dirTyp, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPortTypeName PROTO((vhPortType portTyp, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetSigKindName PROTO((vhSignalKind sigKnd, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetSubProgTypeName PROTO((vhSubProgType typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetAttrbTypeName PROTO((vhAttrbType typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetTypeTypeName PROTO((vhTypeType typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetStaticTypeName PROTO((vhStaticType typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetVhEntityClassName PROTO((vhEntityClass typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPureTypeName PROTO((vhPureType typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetOpCategoryName PROTO((vhOpCategory typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetResolutionMethodName PROTO((vhResolutionMethod typ, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhMakeDistinctElaborationWithBlockConfig PROTO((vhNode nameSpace, vhString lib, vhString ent, vhString arch, int *status, vhNode pBlockConfig, vhString file, int line));

    /* Jaguar Partial Elaborator PI routine prototypes. */
WIN_DLL_EXPORT    void  VhKeepSubTypeIndOfTypeDeclInElab PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void  VhCheckBindingIndInMixFlow PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhElaborateSizeNode PROTO((vhNode object, vhConstString file, int line));
WIN_DLL_EXPORT    void  VhDontElabNonStatExprSizes PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean    vhElaborateExprSizes PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhElaborateNullRange PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    VhUpdateGenericsInEntityContainer PROTO((vhNode pNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhEvaluateExpr PROTO((vhExpr expression, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhCustomEvaluateExpr PROTO((vhExpr expr, int flag, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhOpenDeclarativeRegionNoFolding PROTO((vhNode obj, int flag, vhConstString file, int line));
                                    /* static expression evaluator PI */
WIN_DLL_EXPORT    void    VhSetNonStatArgExprEval PROTO((vhBoolean flag, vhConstString file, int line)); 
WIN_DLL_EXPORT void VhStopConstPropagationInFuncEval PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr  VhEvaluateOperator PROTO((vhOpType Op, vhExpr first, vhExpr second, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhEvaluatePartialStatic PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhStoreDynamicValue PROTO((vhExpr expr, vhNode value, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetDynamicValue PROTO((vhNode obj, vhConstString file, int line)); 
WIN_DLL_EXPORT    void VhSetRecursiveEvaluationDepth PROTO((int depth, vhConstString file, int line));
WIN_DLL_EXPORT    void VhPutDefaultInitialValue PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenInstance PROTO((vhNode instance,vhNode conf_block, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhElaborateBlackBox PROTO((vhNode instance, vhNode OpenInstanceInfo, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode     VhUnElaborateBlackBox PROTO((vhNode instance, vhNode OpenInstanceInfo, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhElaborateStatement PROTO((vhNode stmt, vhBoolean reportMessages , vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhStatementNeedsElaboration PROTO((vhNode stmt, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode  vhElaborateEntity PROTO((vhString entName, vhString archName, vhString libName, vhList genericValList, vhList portValList));

WIN_DLL_EXPORT    vhNode  vhGetUnElaboratedInitialValue PROTO((vhNode port));
WIN_DLL_EXPORT    void    vhAllowUnboundInstance PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhEnableMixedInterface PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhAllowMixedLanguageSupport PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhGiveErrorForGenSubProgDecl PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    VhAllowPragmaOnChainDecl PROTO((vhBoolean flag,vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean    VhIsCallBackEntity PROTO((vhNode ent,vhConstString file, int line));
WIN_DLL_EXPORT    void    VhAllowEntityWithOutArchInElab PROTO((vhBoolean flag,vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetChainDeclFromDirective PROTO((vhNode direc,vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetBlockCommentFromRegionMarker PROTO((vhNode direc,vhConstString file, int line));
WIN_DLL_EXPORT    void    vhAllowNonStaticChoiceInAggregate PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhAllowNonStaticChoices PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vhExpr  VhReplaceOpensInActual PROTO((vhNode formalDecl, vhExpr actual, vhConstString file, int line));
WIN_DLL_EXPORT    void    vhForceBlackbox PROTO((vhBoolean flag));
WIN_DLL_EXPORT    int     vhIsBlackBoxed PROTO((vhNode OpenInstanceInfo));
WIN_DLL_EXPORT    vhNode  VhGetElabEntity PROTO((vhNode OpenInstanceInfo, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetElabArch PROTO((vhNode OpenInstanceInfo, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhGetElabBlockConfig PROTO((vhNode OpenInstanceInfo, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhFindInBlockConfig PROTO((vhNode instance,vhNode config, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetStatus PROTO((vhNode OpenInstanceInfo, vhConstString file, int line));

WIN_DLL_EXPORT    vhEEAddress VhEvaluateToEEAddress PROTO((vhExpr expr, ConstString file, int line));
WIN_DLL_EXPORT    void    VhDeleteEEAddress PROTO((vhEEAddress &addr, ConstString file, int line));
WIN_DLL_EXPORT    vhEEAddress VhPropagateValue_new PROTO((vhExpr target, vhExpr source, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr VhGetExprFromEEAddress PROTO((vhEEAddress addr, vhExpr orgExpr, vhBoolean createNstatAgg, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhPrintEEAddress PROTO((vhEEAddress eeAddr));
WIN_DLL_EXPORT    int     VhOpenDeclarativeRegion PROTO((vhNode scope, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhOpenDeclarativeRegionFuncCall PROTO((vhNode funcCall));
WIN_DLL_EXPORT    void    VhCloseRegionFuncCall PROTO((vhNode funcCall));
WIN_DLL_EXPORT    void    VhUpdateContainer PROTO((vhNode subProgBody));
WIN_DLL_EXPORT    void    VhUpdateAddrContainer PROTO((vhNode scope));
WIN_DLL_EXPORT    vhBoolean VhGetScopeContainsElabOnlyExtName PROTO((vhNode scope));
//WIN_DLL_EXPORT    void    VhSetScopeContainsElabOnlyExtName PROTO((vhNode scope, vhBoolean val));
WIN_DLL_EXPORT    vhEEAddress VhInitializeForIndexAddr PROTO((vhNode forStmt, long initVal));
WIN_DLL_EXPORT    void    VhUpdateForIndexAddr PROTO((vhEEAddress indexAddr, long val));
WIN_DLL_EXPORT    void    VhDeleteForIndexAddr PROTO((vhNode forStmt));
WIN_DLL_EXPORT    vhEEAddress VhInitializeForGenIndexAddr PROTO((vhNode forGenStmt, long initVal));
WIN_DLL_EXPORT    void    VhUpdateForGenIndexAddr PROTO((vhNode forGenStmt, long val));
WIN_DLL_EXPORT    void    VhDeleteForGenIndexAddr PROTO((vhNode forGenStmt));
WIN_DLL_EXPORT    void    VhRestoreForGenIndex PROTO((vhNode forGenStmt, vhExpr index));
WIN_DLL_EXPORT    int     VhOpenBlockHeader PROTO((vhNode block, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenSubProgram PROTO((vhNode subProgCall, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode  VhOpenOverloadedExpr PROTO((vhNode expr, vhConstString file, int line));
WIN_DLL_EXPORT    int     VhGetElaborateStatus PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    int     VhCloseUnit PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    int     VhCloseFeature PROTO((vhConstString file, int line));

    /* Partial elaborator support routines for distinct entity copy */
WIN_DLL_EXPORT    vhList  vhMakeDistinctElaboration PROTO((vhNode nameSpace, vhString lib, vhString ent, vhString arch, int *status));
WIN_DLL_EXPORT    vhList  vhGetDistinctEntityClassList PROTO((vhNode nameSpace, vhString lib, vhString ent));
WIN_DLL_EXPORT    vhList  vhGetDistinctEntityBlockConfigList PROTO((vhString ent, vhString arch, vhString lib));
WIN_DLL_EXPORT    vhNode  vhCreateDistinctEntityCopy PROTO((vhNode storeEnt));
WIN_DLL_EXPORT    vhNode  vhGetDistinctArch PROTO((vhNode storeEnt));
WIN_DLL_EXPORT    void    vhFreeDistinctElabMem PROTO((void));
WIN_DLL_EXPORT    int     vhCloseDsEntityList PROTO((void));
WIN_DLL_EXPORT    int     vhCloseDsEntity PROTO((vhNode entity));
WIN_DLL_EXPORT    vhNode  vhGetDistinctElabEntity PROTO((vhString lib, vhString ent));
WIN_DLL_EXPORT    vhNode vhGetDsEntityFromInstance PROTO((vhNode instance));
WIN_DLL_EXPORT    vhNode  vhGetOrgDsEntity PROTO((vhString lib, vhString ent));
WIN_DLL_EXPORT    vhString vhGetOrgDsEntityName PROTO((vhString lib, vhString ent));
WIN_DLL_EXPORT    vhNode  vhDsNameSpaceCreate PROTO((void));
WIN_DLL_EXPORT    vhBoolean  vhDsEntityIsValid PROTO((vhNode instance));
WIN_DLL_EXPORT    void    vhSetDistinctNameType PROTO ((vhNode nameSapce, vhDistinctNameType nameType));
WIN_DLL_EXPORT    void    vhDsNameSpaceDestroy PROTO((vhNode nameSpace));
WIN_DLL_EXPORT    void    vhSetCurrentDsNameSpace PROTO((vhNode nameSpace));
WIN_DLL_EXPORT    vhNode  vhFindInDsNameSpace PROTO((vhNode nameSpace, vhString lib, vhString ent));

    /* Partial elaborator support routines for FOR-generate */
WIN_DLL_EXPORT    vhBoolean      VhElaborateGenerateRange PROTO((vhNode forGenerateStmnt, vhConstString file, int line));
WIN_DLL_EXPORT    vhExpr    VhGetGenerateIndexValue PROTO((vhNode forGenerateStmnt, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhUpdateGenerateIndex PROTO((vhNode forGenerateStmnt, vhConstString file, int line));
WIN_DLL_EXPORT    void      VhResetGenerateIndex PROTO((vhNode forGenerateStmnt, vhConstString file, int line));


    /* Jaguar PI for extracting meta-comment information from BIF */
WIN_DLL_EXPORT    vhList       VhGetListofMetaComments PROTO((vhNode object, vhConstString file, int line));
WIN_DLL_EXPORT    vhList       VhGetAllDirectives PROTO ((vhNode DesignUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode       VhGetAssociatedObject PROTO((vhNode Directive, vhConstString file, int line));
WIN_DLL_EXPORT    vhString     VhGetAssociatedDirectiveString PROTO((vhNode Directive, vhConstString file, int line));
WIN_DLL_EXPORT    vhList       VhGetRegionMarkerList PROTO((vhNode DesignUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhString     vhGetSynthPrefix PROTO((vhNode regionMarker));
WIN_DLL_EXPORT    void         vhDefineNewRegionMarker PROTO((vhString regionStart, vhString regionEnd, vhRegionType regionType));
WIN_DLL_EXPORT    void         VhUnDefineRegionMarker PROTO((vhString regionStart, vhString regionEnd, vhConstString file, int line));
#ifdef __cplusplus
WIN_DLL_EXPORT    void         vhGetRegionMarkerStrings PROTO((vhNode regionMarker, vhString & prefix, vhString & startMarker, vhString & endMarker));
WIN_DLL_EXPORT    void         vhGetDirectiveStrings PROTO((vhNode directive, vhString & prefix, vhString & directiveString));
#endif
WIN_DLL_EXPORT    vhRegionType   vhGetRegionType  PROTO((vhNode regionMarker));
WIN_DLL_EXPORT    vhRegionType VhIsObjectInRegion PROTO((vhNode Object, vhNode DesignUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean    VhIsValidSynthesisRegion PROTO((vhNode stmt,  vhNode DesignUnit, vhConstString file, int line));

WIN_DLL_EXPORT    vhList       VhGetListofComments PROTO((vhNode DesignUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhString     VhCommentGetString PROTO((vhNode comment, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhSetTranslateOnOffAsComment PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhOverwriteTranslateOnOffAsComment PROTO((char* postDirStr, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhIgnoreTranslateOffOn PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhIgnoreSynthesisOffOn PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhAllowPragmaUnderNoDumpSynth PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhAllowNestedTranslateAndSynthesisPragma PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhSetIgnoreSynthesisRegionDirectiveAsComment PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhSetIgnoreSynthesisRegionDirectiveInsideExprAsComment PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhCompileFuncBodyWithinRegion PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         VhCompileFuncBodyWithinTransOffOnRegion PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void         vhSetRegionMarkerOrder PROTO((vhRegionOrder type));

    /* Jaguar PI for user customized meta-comment support */
WIN_DLL_EXPORT    vhNode       vhCreateUserMetaComment PROTO((vhString name, vhUserMetaCommentType mcType, vhPragmaObjType objType));
WIN_DLL_EXPORT    vhNode       VhCreateRegionMarkerWithBlockComment PROTO((vhString name, vhUserMetaCommentType mcType, vhPragmaObjType objType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode       vhNodeGetPragma PROTO((vhNode obj, vhNode holder));
WIN_DLL_EXPORT    vhList       vhNodeGetAllAttachedPragma PROTO((vhNode obj));
WIN_DLL_EXPORT    void         vhAllowCustomizablePragma PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void         vhReportExistenceOfPragma PROTO((vhBoolean flag));
    /* Jaguar PI to control internal PI messages display */
WIN_DLL_EXPORT    void      VhSetDisplayErrorInJPI PROTO((vhBoolean option, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhGetDisplayErrorOption PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhUserString  VhGetLastOccuredErrMessage PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    int       VhGetLastOccuredErrCode PROTO((vhConstString file, int line));

    /* Jaguar PI to customize the error messages displayed */
WIN_DLL_EXPORT    int vhPrintf PROTO((vhMessageType, int, vhString, int,...));
WIN_DLL_EXPORT    int VhCreateNewMessage PROTO((vhString new_str, vhConstString file, int line));
WIN_DLL_EXPORT    int VhModifyMessage PROTO((int msg_no, vhString new_string, vhConstString file, int line));
WIN_DLL_EXPORT    int VhModifyMsgSeverity PROTO((int msg_no, vhMessageType severity, vhConstString file, int line));
WIN_DLL_EXPORT    void vhSuppressNoFileErrs PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhEnableImplicitOperators PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vhString VhMsgNoGetMnemonic PROTO((int msgNo, vhConstString file, int line));
WIN_DLL_EXPORT    void VhModifyMnemonicMsgSeverity PROTO((vhString mnemonic, vhMessageType new_severity, vhConstString file, int line));
WIN_DLL_EXPORT    void vhGetMessageCount PROTO((int* nErrors, int* nWarnings, int* nRtlErrors));

WIN_DLL_EXPORT    void vhSet87ModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSet2002ModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSet2008ModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSet93ModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetPslModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetNoEmbedPSLModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetPslFileModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetPslv1_01ModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetPsl2005ModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetPslCaseSensKeywordModeOn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhEnableNoDumpInSynth PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhReturnMaximumSize PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhReturnCurrentSize PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhReturnActualSize PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhCalculateSizeForImplicitFunction PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhReturnNullSizeForEvalDependReturn PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhEnable87ConcatBehaviour PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhEnableStrictStaticType PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhEnableErrorRecovery PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhEnableSingleBlackBox PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSetMaximumMessages PROTO((vhMessageType mesgType, int count));

    /* Jaguar dependency PI stuff */
WIN_DLL_EXPORT    vhList VhGetEntityDependencyList PROTO((vhString libName, vhString unitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetVunitDependencyList PROTO((vhString libName, vhString unitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetPackageDependencyList PROTO((vhString libName, vhString unitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetConfigDependencyList PROTO((vhString libName, vhString unitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetPackBodyDependencyList PROTO((vhString libName, vhString unitName, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetArchDependencyList PROTO((vhString libName, vhString entName, vhString archName, vhConstString file, int line));
 
WIN_DLL_EXPORT    int vhDependIsTopUnit PROTO((vhNode dependency));
WIN_DLL_EXPORT    vhString VhDependGetFileName PROTO((vhNode dependency, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhDependGetLibName PROTO((vhNode dependency, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhDependGetUnitName PROTO((vhNode dependency, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhDependGetArchName PROTO((vhNode dependency, vhConstString file, int line));
WIN_DLL_EXPORT    vhObjType VhDependGetUnitType PROTO((vhNode dependency, vhConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhIsEntityUpToDate PROTO((vhString libName, vhString entityName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsArchUpToDate PROTO((vhString libName, vhString entityName,
                                    vhString archName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsConfigUpToDate PROTO((vhString libName, vhString configName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsPackDeclUpToDate PROTO((vhString libName, vhString packName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsPackBodyUpToDate PROTO((vhString libName, vhString packName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean vhIsArchEntityUpToDate PROTO((vhString libName, vhString entityName, vhString archName));
WIN_DLL_EXPORT    VhBoolean VhIsUnInstPack PROTO((vhNode pack,ConstString file, int line));
WIN_DLL_EXPORT    VhBoolean VhIsFuncCallArgCreatedFromDefaultVal PROTO((vhNode expr,ConstString file, int line));


    /* Jaguar design unit time stamp access routines */
WIN_DLL_EXPORT    time_t  vhGetEntityTime PROTO((vhString libName, vhString entityName));
WIN_DLL_EXPORT    time_t  vhGetArchTime PROTO((vhString libName, vhString entityName, vhString archName));
WIN_DLL_EXPORT    time_t  vhGetPackTime PROTO((vhString libName, vhString dunit_name));
WIN_DLL_EXPORT    time_t  vhGetPackBodyTime PROTO((vhString libName, vhString dunit_name));
WIN_DLL_EXPORT    time_t  vhGetConfigTime PROTO((vhString libName, vhString dunit_name));


    /** Jaguar Memory Allocation Control API **/
WIN_DLL_EXPORT    void    vhSetMemoryAllocationByNew PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhSetMemoryAllocationChunkSize PROTO((int size));
WIN_DLL_EXPORT    void    vhDisableNonDumpableMemoryMgr PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void    vhSetForTemporaryExprPlane PROTO((vhBoolean option));
WIN_DLL_EXPORT    void    vhSetTemporaryExprPlaneSize PROTO((int size));
WIN_DLL_EXPORT    void    vhDestroyTemporaryExprPlane PROTO((vhExpr expr));
WIN_DLL_EXPORT    void    vhEnableNodeDeletion PROTO((vhBoolean option));
WIN_DLL_EXPORT    void    VhDeleteNode PROTO((vhNode *expr, vhConstString file, int line));
WIN_DLL_EXPORT    void    VhMakeCreatedNodeDeleteable(VhBoolean flag);

    /** Jaguar analyze PI function **/
WIN_DLL_EXPORT    int     VhJaguarAnalyze PROTO((int argc, vhString* argv, vhConstString file, int line));


    /** Jaguar elaboration time range check routines **/
WIN_DLL_EXPORT    vhBoolean VhRangeCheckOnExpression PROTO((vhExpr expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhRangeCheckOnStatement PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhRangeCheckOnRegion PROTO((vhNode region, vhConstString file, int line));
    /* For any region having a list of concurrent/sequential statements */


    /** Jaguar PI support for extracting fanin/fanout information */
WIN_DLL_EXPORT    void    VhCreateFaninFanoutInfo PROTO((vhNode region, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSignalFaninList PROTO((vhNode sig, vhConstString file, int line));
WIN_DLL_EXPORT    vhList  VhGetSignalFanoutList PROTO((vhNode sig, vhConstString file, int line));
WIN_DLL_EXPORT    int     vhGetVectorSignalSize PROTO((vhNode sig));
                            /* returns 1 for scalar */

    /* Added following new PI routines - Puneet */
WIN_DLL_EXPORT    vhList VhSignalGetFanoutListAtIndex PROTO((vhNode sig, int index,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhSignalGetFaninListAtIndex PROTO((vhNode sig, int index,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhNetBitGetFaninList PROTO((vhNode netbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhNetBitGetFanoutList PROTO((vhNode netbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhNetBitGetNet PROTO((vhNode netbit, vhConstString file, int line));
WIN_DLL_EXPORT    vhUserString VhNetBitGetName PROTO((vhNode netbit, vhConstString file, int line));
WIN_DLL_EXPORT    int VhNetBitGetIndex PROTO((vhNode netbit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhTermBitGetNetBit PROTO((vhNode termbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhTermBitGetPortBit PROTO((vhNode termbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPortBitGetPort PROTO((vhNode portbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPortBitGetNetBit PROTO((vhNode portbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhUserString VhPortBitGetName PROTO((vhNode portbit,
                                    vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhTermBitGetTerminal PROTO((vhNode termbit,
                                    vhConstString file, int line));

    /* New PIs added for NOM compatibility */
WIN_DLL_EXPORT    vhNode VhSignalGetNetBit PROTO((vhNode portbit, int index, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPortGetPortBit PROTO((vhNode portbit, int index, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhTerminalGetTermBit PROTO((vhNode portbit, int index, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode VhTerminalGetPort PROTO((vhNode portbit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhTerminalGetInstance PROTO((vhNode portbit, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhTerminalGetTermBitList PROTO((vhNode portbit, vhConstString file, int line));

WIN_DLL_EXPORT    vhNode VhInstanceGetTerminal PROTO((vhNode portbit, int index, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhInstanceGetCorrespondingTerminal PROTO((vhNode portbit, vhNode formal, vhConstString file, int line));

    /** PI to get regular expression matched object list in symtab **/
WIN_DLL_EXPORT    vhList VhSearchRegularExprInScope PROTO((vhNode scope, vhString regExpr, vhObjType filter, vhConstString file, int line));

    /** PI to change the case of the names reported **/
WIN_DLL_EXPORT    void vhSetNameCaseType PROTO((vhNameCaseType caseTyp));
WIN_DLL_EXPORT    void VhPreserveActualNameInDump PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhSetCheckSumDepCheck PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhSetCheckSumDepCheckWithInfo PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhEnableCheckForOverwrite PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhEnableErrorForOverwrite PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhEnableUnLinkBeforeOverwrite PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhKeepFirstEntArch PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterMvvRemovefromDump(vh_MVV_remove_replace_callback_ptr function, vhConstString file, int line);
WIN_DLL_EXPORT    vhBoolean VhRemoveEntityAndArchFromDump PROTO((vhString libName, vhString entityName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhRemovePackDeclAndPackBodyFromDump PROTO((vhString libName, vhString packName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhRemoveConfigFromDump PROTO((vhString libName, vhString configName, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhRemoveVunitFromDump PROTO((vhString libName, vhString configName, vhConstString file, int line));
WIN_DLL_EXPORT    int VhIsUnitTop PROTO((vhString lib, vhString ent, vhString arch, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhCreateUserPragma PROTO((vhNode obj, vhString prefix, vhString dirStr, vhString value, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhInstanceHasConfigurationBindingAspect PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhInstanceHasMultipleBindings PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    void VhIncludeWorkStd PROTO((vhBoolean obj, vhConstString file, int line));
WIN_DLL_EXPORT    void VhShowPortsWithinSynthRegion PROTO((vhBoolean obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsSignalVector PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsExprVector PROTO((vhNode obj, vhConstString file, int line));

    /** PI to register one signal handler for all signals **/
WIN_DLL_EXPORT    void vhRegisterAllSignalHandlers PROTO((void (*)(int)));

    /** PI to register individual signal handler for individual signals **/
WIN_DLL_EXPORT    void vhDisableSignalHandling PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhUseStructuralLicensing PROTO((vhBoolean flag));
#ifdef WIN32
WIN_DLL_EXPORT    void vhRegisterSignalHandler PROTO((int sigNo, void (* custom_sig_hand_pt)(int)));
#else
WIN_DLL_EXPORT    void vhRegisterSignalHandler PROTO((int sigNo, void (* custom_sig_hand_pt)(void)));
#endif

    /** PI to register user's own message handler **/
WIN_DLL_EXPORT    void vhRegisterMessageHandler PROTO(
            (void (*)(vhString, int, vhMessageType, int, vhString)));
WIN_DLL_EXPORT    void vhRegisterVarArgsMessageHandler PROTO((void (*) /* function pointer */
        (FILE*, vhBoolean, vhMessageType, int, vhString , int, vhString , va_list)));
WIN_DLL_EXPORT    void vhRegisterRMMMessageHandler PROTO((void (*f)
            (vhString, int, vhMessageType, int, vhString, vhNode, va_list)));
WIN_DLL_EXPORT    void vhPassColumnInMsgHandler PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhRegisterTextioMessageHandler PROTO(
            (vh_callback_TextioMessageHandler_ptr));
WIN_DLL_EXPORT    void vhRegisterAttributeSpecHandler PROTO(
            (vh_callback_AttributeSpecHandler_ptr));
WIN_DLL_EXPORT    void vhRegisterIfgenerateMessageHandler PROTO(
            (vh_callback_IfgenerateMessageHandler_ptr));

    /* PI to bypass default handling of .jaguarc by analyzer */
WIN_DLL_EXPORT    void vhDefineLibraryMappings PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhDefineSystemLibraryMappingsOnly PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhJaguarHomeUndefined PROTO((vhBoolean flag));
WIN_DLL_EXPORT    int vhReadVhdlLibraryMappings PROTO((vhString fileName));

WIN_DLL_EXPORT    int VhAddLogicalLibraryToMap PROTO((vhString logicalN, vhString path));
WIN_DLL_EXPORT    int vhCreateLogicalLibraryAlias PROTO((ConstString logicalLib, ConstString aliasLogLib));
WIN_DLL_EXPORT    vhString vhGetAliasNameFromLogLib PROTO((ConstString LogLib));
WIN_DLL_EXPORT    int vhDeleteLogicalLibrary PROTO((vhString logicalN));
WIN_DLL_EXPORT    int VhSetJaguarWorkLibrary PROTO((vhString logicalN));
WIN_DLL_EXPORT    void vhKeepDUsInMemory PROTO((vhBoolean flag));

    /* Configurable support for meta-comments */
WIN_DLL_EXPORT    void vhDefineMetaComments PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhDefineCmdLineSynthPrefix PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhDefineSynthPrefix PROTO((vhString prefix));
WIN_DLL_EXPORT    void vhDefineCombineSynthPrefix PROTO((vhString prefix));
WIN_DLL_EXPORT    void vhDontTreatDirectiveAsComment PROTO((vhString mark1, vhString mark2));
WIN_DLL_EXPORT    void vhUnDefineSynthPrefix PROTO((vhString prefix));
WIN_DLL_EXPORT    void vhUnDefineCombineSynthPrefix PROTO((vhString prefix));
WIN_DLL_EXPORT    void vhAddMetaRegionMarker PROTO((ConstString mark1, ConstString mark2,
                                                            ConstString symbol));
WIN_DLL_EXPORT    void vhAddMetaDirective PROTO((ConstString mark, ConstString symbol));

WIN_DLL_EXPORT    vhNode VhGetAbsoluteBaseType PROTO((vhNode subType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhGetSubTypeBaseType PROTO((vhNode subType, vhConstString file, int line));
    
WIN_DLL_EXPORT    vhBoolean vhDisableJaguarLicenseCheck PROTO((int key1, int key2, int key3));
WIN_DLL_EXPORT    int vhFreeJaguarLicense PROTO((void));
WIN_DLL_EXPORT    int vhLicenseCheckout PROTO((vhNode lmHandler, vhNode feature, int key1, int key2, int key3));
WIN_DLL_EXPORT    int vhFreeJaguarMemory PROTO((void));

    /* PIs related to decompiling a node */
    /* Hash def required for this PI, see at the bottom */
WIN_DLL_EXPORT    void vhDecompileNode PROTO((vhNode node));
//WIN_DLL_EXPORT    char *vhDecompilePackageInVerilog PROTO((vhNode node));
WIN_DLL_EXPORT    vhUserString vhStrDecompileNode PROTO((vhNode node));
WIN_DLL_EXPORT    vhUserString vhEvaluateExprInString PROTO((vhExpr expr));
WIN_DLL_EXPORT    void vhSetDecompileLogFile PROTO((FILE* filePtr)); 
WIN_DLL_EXPORT    int vhDecompileToVhdlFile PROTO((vhString lib, vhString pri, vhString sec, vhString fullWorkPath, vhString skipEntityFileName));
WIN_DLL_EXPORT    void vhEnableDirectiveDecompile(vhBoolean flag);
WIN_DLL_EXPORT    void VhEnableMetaCommentsDecompile(vhBoolean flag);
WIN_DLL_EXPORT    void VhDisableDecompileWithinRegionMarker(vhBoolean flag);
WIN_DLL_EXPORT    void vhSetNamedDecompile(vhBoolean flag);
WIN_DLL_EXPORT    void vhEnableNamedDecompile(vhBoolean flag);
WIN_DLL_EXPORT    void vhBrowseNode PROTO((vhNode obj));
WIN_DLL_EXPORT    vhNode vhOpenDesignUnit PROTO((vhString libname, vhString primName, vhString secName));

WIN_DLL_EXPORT    int vhSetMessagesLogFile PROTO((FILE* logstr));
    
    /* Command line processing PI stuff */
WIN_DLL_EXPORT    int vhcAddCmdLineOption PROTO((vhString option, int numArgs));
WIN_DLL_EXPORT    int vhcProcessCmdLine PROTO((int argc, vhString* argv));
WIN_DLL_EXPORT    int vhcIsOptionActive PROTO((int optionPos));
WIN_DLL_EXPORT    vhString* vhcGetOptionArguments PROTO((int optionPos));
WIN_DLL_EXPORT    vhString* vhcGetCommandArguments PROTO((void));
WIN_DLL_EXPORT    void vhcFreeCmdLineOptions PROTO((void));

WIN_DLL_EXPORT    void   vhSetMixedLanguageCmdLine PROTO((vhBoolean cmdLimeMsgFlag));
WIN_DLL_EXPORT    int    vhProcessCommandLine PROTO((int argc, vhString* argv, vhNode* vhCmdLine));
WIN_DLL_EXPORT    void   vhUpdateCommandLine PROTO((vhNode jagCmdLineObj));

WIN_DLL_EXPORT    void vhEnableInfoDeletetionInSymTabOfDumpedUnits PROTO((vhBoolean));
WIN_DLL_EXPORT    void VhResetGlobalSymTabOfDumpedUnits PROTO((char *,ConstString , int));

    /*  PI for prefix of extended id map file. */

WIN_DLL_EXPORT    void VhSetPrefixForExtIdMapFileInDumpRestore PROTO((vhString, vhList, ConstString , int));

    /*  PI for illegal signal access error in function support */

WIN_DLL_EXPORT    void vhEnableIllegalSigAccessErrorForImpureFunc PROTO((vhBoolean ));

    /*  PI for getting the warning status of an evaluated expr. */

WIN_DLL_EXPORT    vhBoolean VhGetIsSizeMismatchOccuredDuringEval PROTO((vhExpr, ConstString , int ));

    /*  PI for setting flag for context clause decompilation. */

WIN_DLL_EXPORT    void vhEnableDecompilationByContextClause PROTO((vhBoolean ));

    /*  PI for setting flag for embedded PSL in Vunit scope. */

WIN_DLL_EXPORT    void vhAllowEmbeddedPSLInVUnitScope PROTO((vhBoolean ));

    /*  PI for getting sorted graph structures. */

WIN_DLL_EXPORT    void vhDGEnableDependencyGraphCreation PROTO((vhBoolean ));
WIN_DLL_EXPORT    void VhDGFreeFileBasedDependencyGraph PROTO((vhList *, ConstString , int ));
WIN_DLL_EXPORT    vhString VhDGFileInfoGetFileName PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    vhString VhDGFileInfoGetLibName PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    vhList vhGetFileBasedDependencyGraph PROTO((void));
WIN_DLL_EXPORT    vhList VhDGFileInfoGetIncidentFileList PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    unsigned VhDGFileInfoGetTokenCount PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    vhList VhDGFileInfoGetUnResolvedUnitList PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    void VhDGFileInfoGetUnResolvedUnitInfo PROTO((vhNode , char **, char **, char **, ConstString , int ));
WIN_DLL_EXPORT    int VhDGFileInfoGetEffectiveDUCount PROTO((vhNode , ConstString , int ));
WIN_DLL_EXPORT    void VhLogTextEdit PROTO((vhEditType, vhNode, vhNode, ConstString , int ));
WIN_DLL_EXPORT    vhBoolean VhDumpTextEdit PROTO((char *, char *, char *, ConstString , int));
WIN_DLL_EXPORT    void vhFreeJaguarTextEditMemory PROTO((void));

    /*  PI for Psl support */
WIN_DLL_EXPORT    vhList VhGetPslDeclList PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetPslStmtList PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhDuGetPslDeclList PROTO((vhNode du, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhDuGetPslStmtList PROTO((vhNode du, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslLoadVerificationUnit PROTO((vhString sVunitName, vhString sLibName, vhConstString file, int line));
WIN_DLL_EXPORT    vhPslVUnitType VhPslVerifUnitGetUnitType PROTO((vhNode pVerifyUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsDefaultPslVerifUnit PROTO((vhNode pVerifyUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhPslVerifUnitGetUnitName PROTO((vhNode pVerifyUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslVerifUnitGetUnitUnderVerification PROTO((vhNode pVerifyUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslVerifUnitGetInheritedUnitList PROTO((vhNode pVerifyUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslVerifUnitGetItemList PROTO((vhNode pVerifyUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslGetClockExpr PROTO((vhNode pClockDeclOrExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslNamedDeclGetFormalList PROTO((vhNode pNamedDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhPslParamKind VhPslParamGetParamKind PROTO((vhNode pParam, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhPslParamIsConst PROTO((vhNode pParam, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslParamGetSubTypeInd PROTO((vhNode pParam, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhPslDeclGetName PROTO((vhNode pDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslStmtGetDefaultClock PROTO((vhNode pStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslGetProperty PROTO((vhNode pPslObj, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslGetSequence PROTO((vhNode pPslObj, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslFairnessGetExpr PROTO((vhNode pStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslFairnessGetStrongExpr PROTO((vhNode pStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslForAllGetForAllIndex PROTO((vhNode pForAllInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslForAllIndexGetIndexRange PROTO((vhNode pForAllInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslForAllIndexGetValueSet PROTO((vhNode pForAllInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhPslPropOpType VhPslPropertyOperatorGetOpType PROTO((vhNode pPropOp, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslPropertyOperatorGetBound PROTO((vhNode pPropOp, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslExprGetOperand PROTO((vhNode pExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslExprGetLeftOperand PROTO((vhNode pExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslExprGetRightOperand PROTO((vhNode pExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslExprGetMiddleOperand PROTO((vhNode pExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhPslInstanceGetName PROTO((vhNode pInstance, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslInstanceGetMaster PROTO((vhNode pInstance, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslInstanceGetActualParamList PROTO((vhNode pInstance, vhConstString file, int line));
WIN_DLL_EXPORT    vhPslSereRepeatType VhPslSereGetRepeatType PROTO((vhNode pSereExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhPslSereCompositType VhPslSereGetCompositType PROTO((vhNode pSereExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslSereConcatSEREList PROTO((vhNode sereConcat, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhPslFuncCallGetAssociationList PROTO((vhNode pFuncCall, vhConstString file, int line));
WIN_DLL_EXPORT    vhPslBuiltInFuncType VhPslFuncCallGetFuncType PROTO((vhNode pFuncCall, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhPslUnrollForAll PROTO((vhNode pforAll, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhPslUpdateForAll PROTO((vhNode pforAll, vhConstString file,
 int line));
WIN_DLL_EXPORT    vhExpr VhPslForAllGetCurrVal PROTO((vhNode pforAll, vhConstString file
, int line));
WIN_DLL_EXPORT    vhBoolean VhPslResetForAll PROTO((vhNode pforAll, vhConstString file ,
 int line));
WIN_DLL_EXPORT    vhNameType VhPslGetNameTypeOfBindUnitExpr PROTO((vhExpr 
 bindUnitExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhPslGetPrimaryUnitNameOfBindUnitExpr PROTO((vhExpr
bindUnitExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhString VhPslGetSecondaryUnitNameOfBindUnitExpr PROTO((vhExpr
bindUnitExpr, vhConstString file, int line));
WIN_DLL_EXPORT    void VhPslAvoidErrorForUnusedForallIndex PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhPslAllowClockAsIdentifier PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhPslUseFirstDeclForBitVectorValidation PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT    void VhIgnoreSizeMissMatchInIfGen PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT vhBoolean VhIsAPslObj PROTO((vhNode  obj, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhPslGetParametersDefinition PROTO((vhNode pNode, vhConstString file, int line));

    /*  PR 8535: PI for PSL 1.1 Support */
WIN_DLL_EXPORT    vhPslPslExprOpType VhPslPslExprOperatorGetOpType PROTO((vhNode pExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslGetReportString PROTO((vhNode pStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhPslGetSeverityString PROTO((vhNode pStmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhPslSequenceGetIsStrong PROTO((vhNode pPslObj, vhConstString file, int line));

    /*  PI for Mixed Language Flow */
WIN_DLL_EXPORT    void vhRegisterEntityCreateFunction PROTO((vh_callback_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterConfigCreateFunction PROTO((vh_config_callback_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterConfigEntityGetCallBack PROTO((vh_config_entity_callback_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterEntityFlushFunction PROTO((vh_func_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterGenericUpdationFunction PROTO((vh_genericUpdationFuncPtr userFunc));
WIN_DLL_EXPORT    void vhRegisterEntityFindFunction PROTO((vh_entity_exist_callback_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterEinputDecryptFuntion PROTO((vh_einput_decrypt_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterUserFuncForPragma PROTO((vh_userFuncForPragma_ptr userFunc));
WIN_DLL_EXPORT    void vhRegisterFunctionForProfileBegin PROTO((vh_callback_actionOnProfileBegin userFunc));
WIN_DLL_EXPORT    void vhRegisterFunctionForProfileEnd PROTO((vh_callback_actionOnProfileEnd userFunc));

WIN_DLL_EXPORT void vhRegisterFunctionToCheckIsProtectedRegion(vh_callback_actionOnProtectedRegion func);

    /* Generic stack functionality PI */
WIN_DLL_EXPORT    vhStack vhCreateStack PROTO((void));
#ifdef __cplusplus
WIN_DLL_EXPORT    void    vhPushStack PROTO((vhStack &stk, vhNode info));
WIN_DLL_EXPORT    vhNode  vhPopStack PROTO((vhStack &stk));
#endif
WIN_DLL_EXPORT    void    vhDeleteStack PROTO((vhStack stk));
WIN_DLL_EXPORT    int     vhGetStackDepth PROTO((vhStack stk));


    /* Added new PIs for RTLCompiler support */
WIN_DLL_EXPORT    vhNode vhGetAttribOnObjectValue PROTO((ConstString atname, vhNode object));
WIN_DLL_EXPORT    vhNode vhGetAttribSpecOnObject  PROTO((vhString atname, vhNode object));
WIN_DLL_EXPORT    void vhMakeElabSynthChecks PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhDisableLoopRangeCheck PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhPrintMessageGroupHeader PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhSuppressScalarRangeCheck PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhDisableOutofdateCheck PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhKeepAllBlackBoxUnits PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void vhIgnoreFuncVarDefaultInitialValue PROTO((vhBoolean flag));

    /* PIs for pre-processing */
WIN_DLL_EXPORT    vhList vhScan PROTO((int argc, vhString* argv));
WIN_DLL_EXPORT    vhList vhMakeListOfDUs PROTO((int argc, vhString* argv));
WIN_DLL_EXPORT    vhString vhGetPrimNameFromDUInfo PROTO((vhNode node));
WIN_DLL_EXPORT    vhString vhGetSecNameFromDUInfo PROTO((vhNode node));
WIN_DLL_EXPORT    vhString vhGetFileNameFromDUInfo PROTO((vhNode node));
WIN_DLL_EXPORT    int vhGetLineNoFromDUInfo PROTO((vhNode node));
WIN_DLL_EXPORT    vhList vhScanFile PROTO((vhString file_name));
WIN_DLL_EXPORT    vhList vhScanAndSort PROTO((int argc, vhString *argv));
WIN_DLL_EXPORT    int    vhGetScanStatus PROTO((void));
WIN_DLL_EXPORT    void vhUseFileBasisSort PROTO((vhBoolean flag));
WIN_DLL_EXPORT    vhList vhGetSortedFileNames PROTO((void));
WIN_DLL_EXPORT    int    vhAnalyzeDesignUnit PROTO((vhString priName, vhString secName,
                        vhString fileName, vhNode *duAnalyzedRoot));

    /* Allow user to access list of DUs analysed during vhJaguarAnalyze */
WIN_DLL_EXPORT    void vhKeepAnalyzedUnitsList PROTO((void));
WIN_DLL_EXPORT    vhList vhGetAnalyzedUnitsList PROTO((void));
WIN_DLL_EXPORT    vhList vhGetAnalyzedUnitNamesList PROTO((void));
WIN_DLL_EXPORT    vhList vhGetAllAnalyzedUnitNamesList PROTO((void));
WIN_DLL_EXPORT    vhList vhGetAnalyzedDesignUnitNamesList PROTO((void));
#ifdef _DFT_HDLENGINE_
WIN_DLL_EXPORT    vhList Hdle_vhGetAnalyzedDesignUnitNamesList PROTO((int& analysisStatus));
#endif
WIN_DLL_EXPORT    vhList vhGetAllAnalyzedDesignUnitNamesList PROTO((void));
WIN_DLL_EXPORT    vhList vhGetBlackBoxUnitsList PROTO((void));

    /* File analyze PI. */
WIN_DLL_EXPORT    int    vhAnalyzeFile PROTO((vhString fileName, vhList* duAnalyzedList));
WIN_DLL_EXPORT    vhNode  vhAnalyzeNextDesignUnit PROTO((FILE *fp));
WIN_DLL_EXPORT    int vhInitSequentialParser PROTO((vhString duFileName));

    /* List creation and updation PI set */
WIN_DLL_EXPORT    vhList vhwCreateNewList PROTO((vhBoolean));
WIN_DLL_EXPORT    int vhwAppendToList PROTO((vhList, vhNode));
WIN_DLL_EXPORT    int vhwPrependToList PROTO((vhList, vhNode));
WIN_DLL_EXPORT    int vhwDeleteFromList PROTO((vhList, vhNode));
WIN_DLL_EXPORT    int vhwReplaceInList PROTO((vhList, vhNode, vhNode));
WIN_DLL_EXPORT    int vhwReplaceInListPos PROTO((vhList, int, vhNode));
WIN_DLL_EXPORT    int vhwAddAfterNode PROTO((vhList, vhNode, vhNode));
WIN_DLL_EXPORT    int vhwAddBeforeNode PROTO((vhList, vhNode, vhNode));
WIN_DLL_EXPORT    void vhwReplaceList PROTO((vhList, vhList));
WIN_DLL_EXPORT    int vhwFindInList PROTO((vhList mList, vhNode node));
WIN_DLL_EXPORT    int vhAppendToOpenedUnitList PROTO((vhNode unit, vhString lib));

    /* Miscellaneous create PIs */

WIN_DLL_EXPORT    vhNode VhwGetCreateScope PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    int    VhwSetCreateScope PROTO((vhNode scope, vhString filen, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAggregate PROTO((vhList elements, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhCreateProperAggregate PROTO((vhNode aggr, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAlias PROTO((vhString name, vhNode type, vhNode aliasedExpr, vhNode signature, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateGeneric PROTO((vhList genericList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateGenericMap PROTO((vhList associationList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateLabel PROTO((vhString name, vhNode assocStmnt, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateLiteral PROTO((int value, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreatePort PROTO((vhList signalList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreatePortMap PROTO((vhList associationList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSignature PROTO((vhList typeList, vhNode returnType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateString PROTO((vhString string, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateUseObjectFrom PROTO((vhString name, vhNode copy_from, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateWaveFormElement PROTO((vhNode valueExpr, vhNode timeExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateWaveForm PROTO((vhList waveFormElementList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateConfigSpec PROTO((vhNode compSpec, vhNode bindInd, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateConstant PROTO((vhString pconstName, vhNode ptype, vhNode pinitialValue, vhPortType pportType, vhBoolean pgeneric, vhBoolean pinterface, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSignal PROTO((vhString sigName, vhNode type, vhNode initialValue, vhPortType portType, vhSignalKind sigKind, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateExternalName PROTO((vhObjType classType, vhNode subTypeInd,unsigned noOfUpRef,vhExternalPathName pathNameCat, vhList pathNameList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateVariable PROTO((vhString varName, vhNode type, vhNode initialValue, vhPortType portType, vhBoolean isShared, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateFileDecl PROTO((vhString fileDeclName, vhNode type, vhNode logicalName, vhNode openKind, vhPortType openMode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateComponent PROTO((vhString compName, vhList genericList, vhList portList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAttrbDecl PROTO((vhString name, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAttributeSpec PROTO((vhString name, vhNode attrbDecl, vhList entityDesignatorList, vhEntityClass entityClass, vhNode attributeValExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateForIndex PROTO((vhString name, vhNode disRange, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSubProgDecl PROTO((vhString name, vhList parameterList, vhNode returnType, vhSubProgType subProgType, vhNode subProgBody, vhPureType isImPure, vhBoolean overloadedOp, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSubProgBody PROTO((vhString name, vhNode subProgDecl, vhResolutionMethod resolutionMethod, vhList seqStmntList,vhBoolean hasSignal, vhBoolean hasWait, vhBoolean isSynth, vhBoolean isRecursive, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateTypeDecl PROTO((vhString typeName, vhNode typeDef, vhConstString file, int line, VhBoolean internalType));
WIN_DLL_EXPORT    vhNode VhwCreateSubTypeDecl PROTO((vhString subTypeName, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreatePrimaryUnit PROTO((vhString primUnitName, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSecondaryUnit PROTO((vhString secUnitName, vhNode type, vhNode phsicalLit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateEnumElement PROTO((vhString name, vhNode enumLit, vhNode masterType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateElementDecl PROTO((vhString name, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreatePhysicalType PROTO((vhNode rangeCons, vhNode primary, vhList  secondaryList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAccessType PROTO((vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateFileType PROTO((vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateEnumType PROTO((vhList enumList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIntegerType PROTO((vhNode rangeCons, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateFloatType PROTO((vhNode rangeCons, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateConstrainedArray PROTO((int noOfDim, vhNode indexCons, vhNode elementType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateUnConstrainedArray PROTO((int noOfDim, vhList indexList, vhNode elementType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIndexSubTypeDef PROTO((vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateMultiDArraySubtype PROTO((vhNode arrayTypeDecl, vhString subtypeName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRecord PROTO((vhList listOfElements, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSubTypeInd PROTO((vhNode type, vhNode constraint, vhBoolean resolved, vhNode resFunction, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRange PROTO((vhDirType dirType, vhNode leftExpr, vhNode rightExpr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIndexConstraint PROTO((vhList listOfDisRange, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateArrayConstraint PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRecordConstraint PROTO((vhList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRecordElementConstraint PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateRangeConstraint PROTO((vhNode range, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateUnary PROTO((vhOpType opType, vhNode expr, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateBinary PROTO((vhOpType opType, vhNode lhsExpr, vhNode rhsExpr, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateFuncCall PROTO((vhNode master, vhList assocList, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateFuncNode PROTO((vhNode formal, vhNode actual, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateQualifiedExpr PROTO((vhNode type, vhNode expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateTypeConversion PROTO((vhNode type, vhNode expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIndexName PROTO((int noOfDim, vhNode prefix, vhList exprList, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSliceName PROTO((vhNode disRange, vhNode prefix, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSliceNode PROTO((vhNode actual, vhNode range, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreatePhysicalLiteral PROTO((vhNode value, vhNode unitDecl, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateBitString PROTO((vhBaseType base, vhString value, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateOpString PROTO((vhString string, vhOpType opType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAttributeName PROTO((vhAttrbType attrbType, vhNode prefix, vhNode attribute, vhNode suffix, vhNode type, vhNode signature, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateVhObject PROTO((vhNode actualObj, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSelectedName PROTO((vhList selList, vhNode actualObj, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSimpleName PROTO((vhString name, vhNameType nType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateElementAssociation PROTO((vhNode expr, vhList choiceList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateOthers PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAll PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateOpen PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIdEnumLit PROTO((vhString name, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateCharLit PROTO((char ch, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateDecLit PROTO((vhString value, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateBaseLit PROTO((int base, vhString value, vhNode type, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateNullLit PROTO((vhConstString file, int line));
WIN_DLL_EXPORT    void   VhwSetExprBound PROTO((vhNode expr, vhNode bound, vhConstString file, int line));
WIN_DLL_EXPORT    void   VhwSetAliasedExpression PROTO((vhNode object, vhExpr aliasedObj, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAssociation PROTO((vhNode formalPart, vhNode actualPart, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateBindingIndication PROTO((vhNode entityAspect, vhList genMapAspect, vhList portmapAspect, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateEntityAspect PROTO((vhString entityName, vhString archName, vhString configName, vhString libName, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateDesignUnit PROTO((vhString primName, vhString secName, vhString libName, vhObjType unitType, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateComponentInstance PROTO((vhNode compDeclaration, vhList genMapAspect, vhList portMapAspect, vhNode configSpec, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateEntityInstance PROTO((vhString entityName, vhList genMapAspect, vhList portMapAspect, vhString archName, vhString libName, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateConfigurationInstance PROTO((vhString confName, vhList genMapAspect, vhList portMapAspect, vhString libName, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateBlock PROTO((vhNode guardExpr, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    void  VhSetConfigItemList PROTO(( vhNode blockConfig,vhList itemList,vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateProtectedDecl PROTO((vhNode, vhConstString , int ));
WIN_DLL_EXPORT    vhNode VhwCreateProtectedBody PROTO((vhNode, vhConstString , int ));
WIN_DLL_EXPORT    vhNode VhwCreateBlockConfig PROTO(( vhExpr blockSpec,vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateCompConfig PROTO((vhNode compSpec,vhNode bindInd,vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateProcess PROTO((vhList sensitivityList, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateConcProcCall PROTO((vhNode procCall, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateConcSignalAssignment PROTO((vhNode target, vhNode delay, vhNode waveForm, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateCondSignalAssignment PROTO((vhNode target, vhNode delay, vhNode waveForm, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSelectedSignalAssignment PROTO((vhNode target, vhNode selectExpr, vhNode delay, vhList selWaveFormList, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIfGenerate PROTO((vhNode condExpr, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateForGenerate PROTO((vhNode forIndex, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateElsIfGenerate PROTO((vhNode, vhNode , vhNode , vhConstString , int ));
WIN_DLL_EXPORT    vhNode VhwCreateElseGenerate PROTO((vhNode, vhNode , vhConstString , int ));
WIN_DLL_EXPORT    vhNode VhwCreateCaseGenerate PROTO((vhNode, vhNode , vhList, vhConstString , int ));
WIN_DLL_EXPORT    vhNode VhwCreateCaseGenAlternative PROTO((vhNode , vhList, vhConstString , int ));
WIN_DLL_EXPORT    vhNode VhwCreateConcAssertion PROTO((vhNode assertion, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateWait PROTO((vhList sensitivitySignalList, vhNode condExpr, vhNode timeoutExpr, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateVariableAssignment PROTO((vhNode targetExpr, vhNode rhsExpr, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSignalAssignment PROTO((vhNode targetExpr, vhNode waveForm, vhNode delayExpr, vhBoolean isTransport, vhBoolean isReject, vhBoolean isInertial, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSignalForceAssignment PROTO((vhNode targetExpr, vhNode forceExpr, vhPortType forceReleaseMode, int isForceReleaseModeDefaultFlag, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateSignalReleaseAssignment PROTO((vhNode targetExpr, vhPortType forceReleaseMode, int isForceReleaseModeDefaultFlag, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateProcedureCall PROTO((vhNode master, vhList assocList, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateIfStmt PROTO((vhNode condExpr, vhList thenList, vhList elsIfList, vhList elseList, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateElsIfStmt PROTO((vhNode condExpr, vhList seqStmtList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateElseStmt PROTO((vhList seqStmtList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateCaseStmt PROTO((vhNode condExpr, vhList caseAlternativeList, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateCaseAlternative PROTO((vhList choiceList, vhList seqStmtList, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateLoopStmt PROTO((vhList seqStmtList, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateWhileStmt PROTO((vhNode condExpr, vhNode loopStmt, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateForStmt PROTO((vhNode forIndex, vhNode loopStmt, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateAssertion PROTO((vhNode condExpr, vhNode reportExpr, vhNode sevExpr, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateReport PROTO((vhNode reportExpr, vhNode sevExpr, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateNext PROTO((vhNode nextLabel, vhNode condExpr,vhNode affectedLoop, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateExit PROTO((vhNode exitLabel, vhNode condExpr,vhNode affectedLoop, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateReturn PROTO((vhNode returnValue, vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateNull PROTO((vhNode label, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhwCreateLibraryClause PROTO((vhNode simpleName, vhConstString file, int line));
WIN_DLL_EXPORT    int    vhOrder PROTO((vhString inputFile, vhString sortedFileName,  vhBoolean analyze));
WIN_DLL_EXPORT    void   vhKeepUndefinedLibrary PROTO((vhBoolean flag));
WIN_DLL_EXPORT    void   vhPreserveLibraryMapping PROTO((vhString fileName));
    /* Mvv related APIs                  */
WIN_DLL_EXPORT    vhBoolean vhIsUnconsGeneric PROTO((vhNode generic));
WIN_DLL_EXPORT    vhUserString vhGetModifiedUnitName PROTO((vhString unitName, vhString logicalLibName));
WIN_DLL_EXPORT    vhBoolean VhIsSignalVector PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsExprVector PROTO((vhNode obj, vhConstString file, int line));
WIN_DLL_EXPORT    void VhSetPrefixForArchElaborateGenerateStatement PROTO((vhString sPrefix, vhConstString file, int line));

    /* PI for printing debug information */
WIN_DLL_EXPORT    void vhPrintPICalls(int debugLevel); 
WIN_DLL_EXPORT    void vhEnablePrintingAbsPathFileName PROTO((vhBoolean type));
WIN_DLL_EXPORT    vhBoolean VhArchElaborateGenerateStatement PROTO((vhNode pArch, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhArchUnElaborateGenerateStatement PROTO((vhNode pArch, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode vhNodeCopy PROTO((vhNode node, vhString new_name, int flag, void *dummy));
WIN_DLL_EXPORT    vhNode vhNodeCopyVUnitUseClauseAsDeclItem PROTO((vhNode node, vhString new_name, int flag, void *dummy));
WIN_DLL_EXPORT    vhBoolean VhRegisterDumpFileHandler PROTO((vh_callback_DumpFileHandler_ptr function, vhConstString file, int line));
WIN_DLL_EXPORT void VhCopyFileNameAndLineNo PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 7722 */
WIN_DLL_EXPORT void VhDontOmitFileOnNameClash PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 7802*/
WIN_DLL_EXPORT void VhSearchAllLibsForUnresolvedRefs PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhCreateDistinctBlackBox PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetLibListForUnresolvedEntities PROTO((vhString duName,vhConstString file, int line));
WIN_DLL_EXPORT void VhSetFuncCallSize PROTO((vhExpr funcCall,vhExpr returnSize,vhConstString file, int line));

/* PI added to fix PR 7538 ---- */

WIN_DLL_EXPORT    void vhAddCreatedObjectsToScope PROTO((int flag));

/* PI to set the vhGi_detailCompatibilityErrMsgIn87Mode flag for 
   detailed error messages in 87 mode : fix for PR 7528  */
WIN_DLL_EXPORT    void vhDetailCompatibilityErrMsgIn87Mode PROTO((vhBoolean flag ));

/* PI added for PR 7722 */
WIN_DLL_EXPORT void VhDontOmitFileOnNameClash PROTO((vhBoolean flag, vhConstString file, int line));

/* PIs added for PR 7735 */
WIN_DLL_EXPORT void VhKeepNamedAssociationInfo PROTO((vhConstString file, int line));
WIN_DLL_EXPORT void VhPreserveSubProgAssocListInfo  PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetSubProgAssocListWithNamedInfo PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetInstanceGenericListWithNamedInfo PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetInstancePortListWithNamedInfo PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT vhBoolean VhIsNamedAssocNode PROTO((vhNode assoc, vhConstString file, int line));

/* PI added for PR 7881 */
WIN_DLL_EXPORT void VhExplicitMsgForMultipleDecl  PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 9495 */
WIN_DLL_EXPORT void VhMsgForMultipleAttributeDecl  PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 9834 */
WIN_DLL_EXPORT void VhDecompileElaboratedLocStatRange  PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 9730 */
WIN_DLL_EXPORT void VhIgnoreChoiceOutOfRange PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhIgnoreMatchingCaseChoiceSemanticCheck PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhIgnoreMatchingCaseSemanticCheckInsideGenerate PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhIgnoreMatchingCaseChoiceSemanticCheckInElab PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhIgnoreMatchingSelectedAsgnSemanticCheck PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhIgnoreMatchingSelectedAsgnSemanticCheckInsideGenerate PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhIgnoreMatchingSelectedAsgnSemanticCheckInElab PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 10035. This PI is customer specific PI */
WIN_DLL_EXPORT vhBoolean VhIsAnalyzingProtectedRegion PROTO((vhConstString file, int line));
WIN_DLL_EXPORT void VhEnableEscapeSeqChar PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhCheckForSameSubTypeIndAndDeclName PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhAllowPslEndpointIn2005Mode PROTO((vhBoolean flag, vhConstString file, int line));
/* JAG-3025 */
WIN_DLL_EXPORT vhBoolean VhIsLinkedToForeignAttr PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT vhBoolean VhIsMatchingCaseStmt PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT void VhSetMatchingCaseStmt PROTO((vhNode node, vhBoolean flag, vhConstString file, int line));
/* JAG-3137 */
WIN_DLL_EXPORT vhBoolean VhIsLinkedToStdForeignAttr PROTO((vhNode node, vhConstString file, int line));
/* PI added for JAG-2963 */
WIN_DLL_EXPORT void VhDisableObjectReuse PROTO((vhBoolean flag, vhConstString file, int line));
/* JAG-2892. This PI is customer specific PI */
WIN_DLL_EXPORT void VhAllowRecordTypeNamePrefixInSelectedName PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 14960(changed in JAG-3460). This PI is customer specific PI */
WIN_DLL_EXPORT void VhIncludeReportSeverityInSelectedPslDirectives PROTO((vhBoolean flag, vhConstString file, int line));

/* PI added for PR 7881 */
WIN_DLL_EXPORT void VhExplicitMsgForMultipleDecl  PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT vhTypeDefConstraintType VhTypeGetConstraintType PROTO((vhNode ,ConstString , int ));

/* PI added for PR 14130. This PI is customer specific PI */
WIN_DLL_EXPORT void VhCheckDeclParamAlsoInSubprogUsesGlobals PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 13102. This PI is customer specific PI */
/* PI added for PR 12073. This PI is customer specific PI */
WIN_DLL_EXPORT void VhEnableSemicolonSeqChar PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 11511. This PI is customer specific PI */
WIN_DLL_EXPORT void VhDontAllowTwoPrimaryDUsWithSameNameInLibrary PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 11415. This PI is customer specific PI */
WIN_DLL_EXPORT void VhAllowUnconnectedFormalPort PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR-12661. This PI is customer specific PI */
WIN_DLL_EXPORT void VhEnablePredefinedAttribSynthCheckStd2004 PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 10283 */
WIN_DLL_EXPORT void VhForcePortInitValueCopy PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 10521. This PI is customer specific PI */
WIN_DLL_EXPORT void VhAllowNonSignalElementInPortMapAspectIn87Mode PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhAllow87ModeSubProgOverloading PROTO((vhBoolean flag, vhConstString file, int line));

/* PI added for PR 10538. These PI is customer specific PI */
WIN_DLL_EXPORT long VhGetBucketNumberForNode PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT vhNode VhGetNodeFromBucketNumber PROTO((long bucketNumber, vhString libraryName, vhObjType scopeType, vhString unitName, vhString primaryUnitName, vhConstString file, int line));
/* PI added for PR 13009. These PI is customer specific PI */
WIN_DLL_EXPORT void VhSetMaxIdentifierLimit PROTO((int idlimit, vhConstString file, int line));

/* PI added for PR 7887 */
WIN_DLL_EXPORT    vhExpr  VhGetPslPercentIfCondition PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPslPercentIfThenText PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT    vhString  VhGetPslPercentIfElseText PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT vhString VhGetPslPercentForReplicatorName PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT vhString VhGetPslPercentForText PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT vhBoolean VhIsPslPercentForReplicatorARange PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetPslPercentForReplicatorValues PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhDuGetPslMacroList PROTO((vhNode du, vhConstString file, int line));
WIN_DLL_EXPORT    vhList VhGetPslMacroList PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT void VhUseWorkForExt PROTO((vhBoolean flag, vhConstString file, int line));
/* PI added for PR 8049 */
WIN_DLL_EXPORT void VhSetEntityNameForCopiedArch PROTO((vhNode arch,vhString entName,vhConstString file,int line));
WIN_DLL_EXPORT void VhSetMraNameForCopiedEntity PROTO((vhNode ent,vhString mraName,vhConstString file,int line));
WIN_DLL_EXPORT void VhSetEntityForArchCopy PROTO((vhNode ent,vhConstString file,int line));
WIN_DLL_EXPORT vhBoolean VhAliasSubprogram PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhIgnoreElabSizeMismatch PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowSizeMismatchInElabAnalysis PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhPickMostRecentConfig PROTO((vhBoolean flag, vhConstString file,int line));
/* PI added for PR 8126: VUnit Elaboarton support */
WIN_DLL_EXPORT vhString VhGetPslVUnitBoundUnitLibraryName PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT vhExpr VhGetPslVUnitBoundUnitDesignUnit PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetPslVUnitBoundUnitInstanceList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT void VhPslBindVerificationUnitDuringRestore PROTO((void));
WIN_DLL_EXPORT void VhPslUnBindVerificationUnitDuringRestore PROTO((void));
WIN_DLL_EXPORT vhList VhPslDUGetBindedVerificationUnitNameList PROTO((vhNode, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhPslInstanceGetBindedVerificationUnitNameList PROTO((vhNode, vhNode, vhString path, vhConstString file, int line));
WIN_DLL_EXPORT void VhPslElabBindedVerificationUnit PROTO((vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhPslInstanceGetBindedVUnitNameListUsingElabEntArch PROTO((vhNode, vhNode, vhNode, vhString path, vhConstString file, int line));
WIN_DLL_EXPORT void VhPslElabBindedVUnitUsingElabEntArch PROTO((vhNode, vhNode, vhNode, vhConstString file, int line));
WIN_DLL_EXPORT vhNode VhGetCurrentDesignUnitNode PROTO((vhConstString file,int line));
WIN_DLL_EXPORT void VhOverrideGenericValue PROTO((vhNode inst, vhNode generic, vhNode value, vhConstString file,int line));
WIN_DLL_EXPORT void VhResetOverriddenGeneric PROTO((vhNode inst, vhNode generic, vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableGenericOverride PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowConstantActualForAllPortTypes PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhCheckForStaticGenVal PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT vhBoolean  VhBlockIsGenerateBlock PROTO((vhNode block, vhConstString file, int line));
WIN_DLL_EXPORT vhList VhGetReadSignalsNotInSensList PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT vhUserString VhBlockGetOriginalName PROTO((vhNode block, vhConstString file, int line));
WIN_DLL_EXPORT int  VhBlockGetGenerateIndex PROTO((vhNode block, vhConstString file, int line));
WIN_DLL_EXPORT void VhEnableSingleLineErrorMessage PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableUnitClosureDuringTopDetection PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void  VhEnableEntHeaderInOpenDeclRegn PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void  VhAllowBackWardCompatibility PROTO((vhBackWardCompatibility obj, vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhEnableSymTabAddition PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableSynthErrWarnUnderExtMsgHandler PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhCreateDsEntBlockConfigList PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableRtlSynthesisOffOnAsSynthesisRegion PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT void VhSetEntPatternToIgnoreTransOffOn PROTO((vhString modName, vhConstString file,int line));
WIN_DLL_EXPORT void VhSetActualImpurity PROTO((vhBoolean flag , vhConstString file,int line));
    /** PR 9599: PI to register user's preParse File handler.
        This API is added in reponse to a special customer request.
        It would be used through MVV **/
WIN_DLL_EXPORT    void VhRegisterPreParseFileHandler PROTO((vh_preParseFileHandlePtr userFunc, int langType, void *userData, vhConstString file, int line));
WIN_DLL_EXPORT void VhUseCreateFileNameForNewNodes PROTO((vhBoolean flag, vhConstString file,int line));
WIN_DLL_EXPORT    void  VhRegisterProtectDecryptionCallBack PROTO((vh_protectDecryptionRegionHandler userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void  VhRegisterProtectNextDecryptionCallBack PROTO((vh_protectNextDecryptionRegionHandler userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterProtectedInputFilter PROTO((vh_protectedRegionHandler userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterDeallocatorForProtectedInputText PROTO((vh_protectedTextDeallocator userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterEvaluateRtlc PROTO((vh_evaluateCallBackRtlc userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterEvaluateRtlcToCheckForNonStaticExpr PROTO((vh_evaluateCallBackRtlcToCheckForNonStaticExpr userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterCommentForProtectedInputText PROTO((vh_protectedTextComment userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterCallbackForEndDU PROTO((vh_CallbackForEndDU userFunc, vhConstString file, int line));
WIN_DLL_EXPORT    void VhRegisterCallbackForEndPackage PROTO((vh_CallbackForEndPackage userFunc, vhConstString file, int line));
WIN_DLL_EXPORT void VhDefineProtectedRegionMarkers PROTO((vhString regionStart, vhString regionEnd, vhConstString file, int line));
WIN_DLL_EXPORT vhBoolean VhIsEncrypted PROTO((vhNode node, vhConstString file, int line));
WIN_DLL_EXPORT void VhSortDsEntities PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhEnablePSLElaborationInDsElab PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhDontStoreDesignUnitComments PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhIgnorePslConstructs PROTO((vhBoolean flag , vhConstString file, int line));
WIN_DLL_EXPORT void VhAllowMultipleNameSpace PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhCheckLogicalAndPhyLibToOverwriteDU PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowNullRanges PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableInMemoryOutOfDateCheck PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhCreateSignedDecLitForIntMin PROTO((vhBoolean flag , vhConstString file,int line));
WIN_DLL_EXPORT void VhRegisterIPDumpEncrypter PROTO((vh_IPDumpEncryptionPrototype function , vhConstString file, int line));
WIN_DLL_EXPORT void VhRegisterIPDumpDecrypter PROTO((vh_IPDumpDecryptionPrototype function , vhConstString file, int line));
WIN_DLL_EXPORT void VhRegisterIPDumpEncryptedTextDeallocator PROTO((vh_IPDumpEncryptedTextDeallocatorProto function , vhConstString file, int line));
WIN_DLL_EXPORT void VhRegisterIPDumpDecryptedTextDeallocator PROTO((vh_IPDumpDecryptedTextDeallocatorProto function , vhConstString file, int line));
WIN_DLL_EXPORT vhNode VhGetProtectKeyValue PROTO((vhNode ,vhConstString file,int line));
WIN_DLL_EXPORT vhProtectKeyType VhGetProtectKeyType PROTO((vhNode ,vhConstString file,int line));
WIN_DLL_EXPORT void VhRegisterEinputDecryptBufferDeallocator PROTO ((vh_einput_decrypt_buffer_deallocator_ptr function, vhConstString file, int line));
WIN_DLL_EXPORT void VhCreateDSEntForTop PROTO((vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowImplicitRead PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhDsElabOptimizationForNonInstance PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhEnable32bitTo64bitConversion PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void vhRegisterCustomDecomInfo PROTO((void (*)(FILE *,vhNode)));
WIN_DLL_EXPORT void vhRegisterPackInstFilePathHandler PROTO((char * (*)(char *)));
WIN_DLL_EXPORT void VhSetFileNameForNewNode PROTO((vhNode newNode,vhString newFileName,vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableInternalEvalExprDeletion PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableRangeConstraintElab PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT vhBoolean VhHasProtectedRegion PROTO((vhNode node ,vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowOutPortRead PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowProcessAll PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowCondAssign PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableEvalPeakMemOpt PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT vhBoolean VhIsNextNodeComment PROTO((vhNode newNode,vhConstString file,int line));
WIN_DLL_EXPORT void VhKeepNextCommentInfo PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT int VhGetGenerateBlockType PROTO((vhNode node ,vhConstString file,int line));
WIN_DLL_EXPORT void VhTreatSynthOffOnSameInAnalAndElab PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowNonStaticUnaryExprInPortMap PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowNonStaticExprInPortMap PROTO((vhBoolean flag ,vhConstString file,int line));
WIN_DLL_EXPORT void VhFreeDependencyUnitList PROTO((vhList *depUnitList,vhConstString file,int line));
WIN_DLL_EXPORT void VhKeepNamedAssocForSingleElemRecAggrInOpenDecl PROTO((vhBoolean flag,vhConstString file,int line));
WIN_DLL_EXPORT void VhSetLineNumberFileNameFormat PROTO((char * format ,vhConstString file,int line));
WIN_DLL_EXPORT void VhSetUserInputFileName PROTO((char * filename ,vhConstString file,int line));
WIN_DLL_EXPORT void VhEnableCopyDumpInLongPath PROTO((vhBoolean flag,vhConstString file,int line));
WIN_DLL_EXPORT void VhAllowMissingBindingForArchNameModule  PROTO((vhBoolean flag,vhConstString file,int line));
WIN_DLL_EXPORT void VhSetNewLocaleImplementation  PROTO((vhBoolean val,vhConstString file,int line));
WIN_DLL_EXPORT vhBoolean VhIsProcessHasAll PROTO((vhNode process, vhConstString file, int line));
WIN_DLL_EXPORT vhPortType  VhGetForceReleaseMode PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT vhBoolean  VhIsForceReleaseModeDefault PROTO((vhNode stmt, vhConstString file, int line));
WIN_DLL_EXPORT vhExpr  VhGetForceExpr PROTO((vhNode stmt,vhConstString file, int line));

WIN_DLL_EXPORT vhBoolean VhHasMatchingSelSig PROTO((vhNode ,vhConstString , int ));
WIN_DLL_EXPORT vhList    VhGetSelectedExprList PROTO((vhNode ,vhConstString , int ));
WIN_DLL_EXPORT vhList    VhGetConditionalExprList PROTO((vhNode ,vhConstString , int ));
WIN_DLL_EXPORT void vhEnableElaborationForLocallyStaticRange PROTO((vhBoolean flag));
WIN_DLL_EXPORT void VhAllowReverseRangeDecompile PROTO((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhAllowDecompileAllLogicalOpToPlus PROTO((VhBoolean flag, ConstString file, int line));

WIN_DLL_EXPORT    vhBoolean VhIsDUStructural PROTO((vhNode designUnit, vhConstString file, int line));
WIN_DLL_EXPORT    String VhGetPackInstDeclName PROTO((vhNode packInst, ConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsExprFromDumpPlane PROTO((vhNode expr, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsPackFromPackInstance PROTO((vhNode designUnit, vhConstString file, int line));
WIN_DLL_EXPORT    vhNode VhSearchSubProgFromAttr PROTO((vhNode prefix, vhNode signature, vhConstString file, int line));
WIN_DLL_EXPORT    vhBoolean VhIsTypeDependentOnGenericType PROTO((vhNode type, vhConstString file, int line));

#ifdef _DFT_HDLENGINE_
WIN_DLL_EXPORT    vhBoolean VhIsDUStructural PROTO((vhNode designUnit, vhConstString file, int line));
#endif

WIN_DLL_EXPORT void VhRegisterParseFOpenAndFCloseCallback PROTO((vh_callback_parse_fopen parseFOpen, vh_callback_parse_fclose parseFClose, vhConstString file, int line));

WIN_DLL_EXPORT void VhCreateNewContainerForAllScopes PROTO ((void *obj, bool bCreateEntityContainer, vhConstString file, int line));
WIN_DLL_EXPORT void VhPopContainerForAllScopes PROTO ((void *obj, vhConstString file, int line));
WIN_DLL_EXPORT void VhRemoveAndGetTopFromStackForAllScope PROTO ((void *obj, vhNode &_addrContainerQueue, vhNode &_typeInfoContainerQueue, vhConstString file, int line));
WIN_DLL_EXPORT void VhPushIntoStackForAllScope PROTO ((void *obj, vhNode &_addrContainerQueue, vhNode &_typeInfoContainerQueue, vhConstString file, int line));
WIN_DLL_EXPORT bool VhIsRecursivelyInstantiated PROTO ((void *obj, vhConstString file, int line));

WIN_DLL_EXPORT vhNode VhwCreateRangeFromIntValues PROTO ((int left, int right));

WIN_DLL_EXPORT void VhSetConsiderInitValOfSigOneAllowedIntValue PROTO ((vhNode signal, vhNode val, vhConstString file, int line));
#ifdef _DFT_HDLENGINE_
WIN_DLL_EXPORT void VhForceEvaluateVhDeclLit PROTO ((vhBoolean val, vhConstString file, int line));
WIN_DLL_EXPORT vhNode VhIsUnelaboratedRangeParameterised PROTO ((vhNode obj, vhConstString file, int line));
#endif

WIN_DLL_EXPORT void VhSetStdLogicVec_StdULogicVec_2008mode PROTO ((vhBoolean val, vhConstString file, int line));
WIN_DLL_EXPORT void VhSetSameDeclOverloadingAllowed PROTO ((vhBoolean val, vhConstString file, int line));
WIN_DLL_EXPORT void VhImportVhdlPackageToLowerNameMixedSvVh PROTO ((vhBoolean flag, vhConstString file, int line));
WIN_DLL_EXPORT void VhRegisterGFileGenericValueCallBack(vh_callback_createVhExprBasedOnValueFromGTable createVhExprBasedOnValueFromSmallGTable, vh_callback_createVhExprBasedOnValueFromGTable createVhExprBasedOnValueFromCapitalGTable, vhConstString file, int line);
WIN_DLL_EXPORT void VhSetGFileNotEmptyFlags(vhBoolean smallGTableIsNotEmpty, vhBoolean capitalGTableNotEmpty, vhConstString file, int line);
WIN_DLL_EXPORT void VhRegisterGFilePushPopForGenerateBlock(vh_callback_mvvElaborateScopeWithGfile  elaborateScopeWithGfile, vh_callback_mvvUnElaborateScopeWithGfile unElaborateScopeWithGfile, vhConstString file, int line);
WIN_DLL_EXPORT void VhRegisterNewConfigFlowCallbacks(vh_callback_mvvProcessingBeforeElabForConfig userFunc1, vh_callback_mvvProcessingAfterUnElabForConfig userFunc2, vhConstString file, int line);
WIN_DLL_EXPORT void VhSetIsNewConfigFlow PROTO ((vhBoolean val, vhConstString file, int line));

#ifdef __cplusplus
}
#endif

#endif /* end JPI include file __JAGUAR_PI_H_ ]] */


#define vhGetObjectFileName(arg1)  VhGetObjectFileName((arg1),JAG_FILE,__LINE__)
#define vhInstanceElabObjSetMaster(arg1, arg2)  VhInstanceElabObjSetMaster((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetSourceFileName(arg1)  VhGetSourceFileName((arg1),JAG_FILE,__LINE__)
#define vhGetAbsoluteSourceFileName(arg1)  VhGetAbsoluteSourceFileName((arg1),JAG_FILE,__LINE__)
#define vhGetObjType(arg1)  VhGetObjType((arg1),JAG_FILE,__LINE__)
#define vhGetLineNumber(arg1)  VhGetLineNumber((arg1),JAG_FILE,__LINE__)
#define vhGetColumnNumber(arg1)  VhGetColumnNumber((arg1),JAG_FILE,__LINE__)
#define vhGetEndLineNumber(arg1)  VhGetEndLineNumber((arg1),JAG_FILE,__LINE__)
#define vhGetEndColumnNumber(arg1)  VhGetEndColumnNumber((arg1),JAG_FILE,__LINE__)
#define vhGetIdentLineNumber(arg1)  VhGetIdentLineNumber((arg1),JAG_FILE,__LINE__)
#define vhGetIdentColumnNumber(arg1)  VhGetIdentColumnNumber((arg1),JAG_FILE,__LINE__)
#define vhGetIdentEndLineNumber(arg1)  VhGetIdentEndLineNumber((arg1),JAG_FILE,__LINE__)
#define vhGetIdentEndColumnNumber(arg1)  VhGetIdentEndColumnNumber((arg1),JAG_FILE,__LINE__)
#define vhGetBeginTokenLineNumber(arg1)  VhGetBeginTokenLineNumber((arg1),JAG_FILE,__LINE__)
#define vhGetBeginTokenColNumber(arg1)  VhGetBeginTokenColNumber((arg1),JAG_FILE,__LINE__)
#define vhGetProcessSensListEndLineNumber(arg1)  VhGetProcessSensListEndLineNumber((arg1),JAG_FILE,__LINE__)
#define vhGetProcessSensListEndColumnNumber(arg1)  VhGetProcessSensListEndColumnNumber((arg1),JAG_FILE,__LINE__)
#define vhGetLibName(arg1)  VhGetLibName((arg1),JAG_FILE,__LINE__)
#define vhGetInstanceAspectLibName(arg1)  VhGetInstanceAspectLibName((arg1),JAG_FILE,__LINE__)

#define vhOpenEntity(arg1,arg2)  VhOpenEntity((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsEntityBlackBox(arg1) VhIsEntityBlackBox((arg1),JAG_FILE,__LINE__)
#define vhOpenArch(arg1,arg2,arg3)  VhOpenArch((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhOpenMRArch(arg1,arg2)  VhOpenMRArch((arg1),(arg2),JAG_FILE,__LINE__)
#define vhHandleOpenArch(arg1,arg2)  VhHandleOpenArch((arg1),(arg2),JAG_FILE,__LINE__)
#define vhHandleOpenMRArch(arg1)  VhHandleOpenMRArch((arg1),JAG_FILE,__LINE__)

#define vhOpenMRConfig(arg1,arg2)  VhOpenMRConfig((arg1),(arg2),JAG_FILE,__LINE__)
#define vhHandleOpenMRConfig(arg1)  VhHandleOpenMRConfig((arg1),JAG_FILE,__LINE__)

#define vhGetContextName(arg1)  VhGetContextName((arg1),JAG_FILE,__LINE__)
#define vhGetEntityName(arg1)  VhGetEntityName((arg1),JAG_FILE,__LINE__)
#define vhGetArchName(arg1)  VhGetArchName((arg1),JAG_FILE,__LINE__)
#define vhOpenContextDecl(arg1,arg2)  VhOpenContextDecl((arg1),(arg2),JAG_FILE,__LINE__)
#define vhOpenPackDecl(arg1,arg2)  VhOpenPackDecl((arg1),(arg2),JAG_FILE,__LINE__)
#define vhOpenPackBody(arg1,arg2)  VhOpenPackBody((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetPackName(arg1)  VhGetPackName((arg1),JAG_FILE,__LINE__)
#define vhOpenConfig(arg1,arg2)  VhOpenConfig((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetConfigName(arg1)  VhGetConfigName((arg1),JAG_FILE,__LINE__)
#define vhIsEntityOpen(arg1,arg2)  VhIsEntityOpen((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsArchOpen(arg1,arg2,arg3)  VhIsArchOpen((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhIsConfigOpen(arg1,arg2)  VhIsConfigOpen((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsPackDeclOpen(arg1,arg2)  VhIsPackDeclOpen((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsPackBodyOpen(arg1,arg2)  VhIsPackBodyOpen((arg1),(arg2),JAG_FILE,__LINE__)
#define vhCloseDesignUnit(arg1)  VhCloseDesignUnit((arg1),JAG_FILE,__LINE__)
#define vhGetLibraryClause(arg1)  VhGetLibraryClause((arg1),JAG_FILE,__LINE__)
#define vhGetLibNameList(arg1)  VhGetLibNameList((arg1),JAG_FILE,__LINE__)
#define vhGetContextRef(arg1)  VhGetContextRef((arg1),JAG_FILE,__LINE__)
#define vhGetUseClause(arg1)  VhGetUseClause((arg1),JAG_FILE,__LINE__)
#define vhGetSelNameList(arg1)  VhGetSelNameList((arg1),JAG_FILE,__LINE__)
#define vhGetNextItem(arg1)  VhGetNextItem((arg1),JAG_FILE,__LINE__)
#define vhGetNextItemAddr(arg1)  VhGetNextItemAddr((arg1),JAG_FILE,__LINE__)
#define vhSetListNodeInfoPtr(arg1,arg2)  VhSetListNodeInfoPtr((arg1),(arg2),JAG_FILE,__LINE__)
#define vhwDeleteNodeAddrFromList(arg1,arg2,arg3)  VhwDeleteNodeAddrFromList((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhGetListNodeNextPtr(arg1)  VhGetListNodeNextPtr((arg1),JAG_FILE,__LINE__)
#define vhGetListNodeInfoPtr(arg1)  VhGetListNodeInfoPtr((arg1),JAG_FILE,__LINE__)
#define vhListGetCurrentNode(arg1)  VhListGetCurrentNode((arg1),JAG_FILE,__LINE__)
#define vhListGetIthNode(arg1,arg2,arg3)  VhListGetIthNode((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhDeleteList(arg1)  VhDeleteList((arg1),JAG_FILE,__LINE__)
#define vhGetArraySize(arg1)  VhGetArraySize((arg1),JAG_FILE,__LINE__)
#define vhListGetLength(arg1)  VhListGetLength((arg1),JAG_FILE,__LINE__)
#define vhListRewind(arg1)  VhListRewind((arg1),JAG_FILE,__LINE__)
#define vhGetNodeAt(arg1,arg2)  VhGetNodeAt((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsResolved(arg1)  VhIsResolved((arg1),JAG_FILE,__LINE__)
#define vhGetResFunc(arg1)  VhGetResFunc((arg1),JAG_FILE,__LINE__)
#define vhGetConstraint(arg1)  VhGetConstraint((arg1),JAG_FILE,__LINE__)
#define vhGetExactConstraint(arg1)  VhGetExactConstraint((arg1),JAG_FILE,__LINE__)
#define vhGetActualOfExternalSize(arg1)  VhGetActualOfExternalSize((arg1),JAG_FILE,__LINE__)
#define vhSetConstraint(arg1,arg2)  VhSetConstraint((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetOrgType(arg1)  VhGetOrgType((arg1),JAG_FILE,__LINE__)
#define vhGetType(arg1)  VhGetType((arg1),JAG_FILE,__LINE__)
#define vhGetPrefixType(arg1)  VhGetPrefixType((arg1),JAG_FILE,__LINE__)
#define vhGetRange(arg1)  VhGetRange((arg1),JAG_FILE,__LINE__)
#define vhGetRangeDir(arg1)  VhGetRangeDir((arg1),JAG_FILE,__LINE__)
#define vhGetLtOfRange(arg1)  VhGetLtOfRange((arg1),JAG_FILE,__LINE__)
#define vhGetRtOfRange(arg1)  VhGetRtOfRange((arg1),JAG_FILE,__LINE__)
#define vhIsRangeAttribute(arg1)  VhIsRangeAttribute((arg1),JAG_FILE,__LINE__)
#define vhGetRangeAttrib(arg1)  VhGetRangeAttrib((arg1),JAG_FILE,__LINE__)
#define vhGetDisRange(arg1)  VhGetDisRange((arg1),JAG_FILE,__LINE__)
#define vhGetTypeName(arg1)  VhGetTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetTypeMark(arg1)  VhGetTypeMark((arg1),JAG_FILE,__LINE__)
#define vhGetRangeConstraint(arg1)  VhGetRangeConstraint((arg1),JAG_FILE,__LINE__)
#define vhGetRecEleName(arg1)  VhGetRecEleName((arg1),JAG_FILE,__LINE__)
#define vhGetEleSubTypeInd(arg1)  VhGetEleSubTypeInd((arg1),JAG_FILE,__LINE__)
#define vhGetRecEleList(arg1)  VhGetRecEleList((arg1),JAG_FILE,__LINE__)
#define vhGetFileType(arg1)  VhGetFileType((arg1),JAG_FILE,__LINE__)
#define vhGetAccessType(arg1)  VhGetAccessType((arg1),JAG_FILE,__LINE__)
#define vhGetTypeDef(arg1)  VhGetTypeDef((arg1),JAG_FILE,__LINE__)
#define vhIsIncType(arg1)  VhIsIncType((arg1),JAG_FILE,__LINE__)
#define vhIsGenType(arg1)  VhIsGenType((arg1),JAG_FILE,__LINE__)
#define vhGetEnumEleList(arg1)  VhGetEnumEleList((arg1),JAG_FILE,__LINE__)
#define vhGetCharVal(arg1)  VhGetCharVal((arg1),JAG_FILE,__LINE__)
#define vhGetCharEnumElement(arg1)  VhGetCharEnumElement((arg1),JAG_FILE,__LINE__)
#define vhGetCharLitValue(arg1)  VhGetCharLitValue((arg1),JAG_FILE,__LINE__)
#define vhIsCharEnumLitElement(arg1)  VhIsCharEnumLitElement((arg1),JAG_FILE,__LINE__)
#define vhGetIdEnumLit(arg1)  VhGetIdEnumLit((arg1),JAG_FILE,__LINE__)
#define vhGetIdEnumLitValue(arg1)  VhGetIdEnumLitValue((arg1),JAG_FILE,__LINE__)
#define vhGetEnumLit(arg1)  VhGetEnumLit((arg1),JAG_FILE,__LINE__)
#define vhGetBaseSpec(arg1)  VhGetBaseSpec((arg1),JAG_FILE,__LINE__)
#define vhGetStringVal(arg1)  VhGetStringVal((arg1),JAG_FILE,__LINE__)
#define vhGetString(arg1)  VhGetString((arg1),JAG_FILE,__LINE__)
#define vhGetBaseLit(arg1)  VhGetBaseLit((arg1),JAG_FILE,__LINE__)
#define vhGetBaseLitValue(arg1)  VhGetBaseLitValue((arg1),JAG_FILE,__LINE__)
#define vhGetNumericValue(arg1)  VhGetNumericValue((arg1),JAG_FILE,__LINE__)
#define vhGetBase(arg1)  VhGetBase((arg1),JAG_FILE,__LINE__)
#define vhGetAbsLit(arg1)  VhGetAbsLit((arg1),JAG_FILE,__LINE__)
#define vhGetUnitDecl(arg1)  VhGetUnitDecl((arg1),JAG_FILE,__LINE__)
#define vhGetPrimUnitDecl(arg1)  VhGetPrimUnitDecl((arg1),JAG_FILE,__LINE__)
#define vhGetSecUnitDeclList(arg1)  VhGetSecUnitDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetPhyLit(arg1)  VhGetPhyLit((arg1),JAG_FILE,__LINE__)
#define vhGetUnitName(arg1)  VhGetUnitName((arg1),JAG_FILE,__LINE__)
#define vhGetPhysicalUnitType(arg1)  VhGetPhysicalUnitType((arg1),JAG_FILE,__LINE__)
#define vhGetSecPrimUnitRatio(arg1)  VhGetSecPrimUnitRatio((arg1),JAG_FILE,__LINE__)
#define vhGetIntPart(arg1)  VhGetIntPart((arg1),JAG_FILE,__LINE__)
#define vhGetFloatPart(arg1)  VhGetFloatPart((arg1),JAG_FILE,__LINE__)
#define vhGetExponent(arg1)  VhGetExponent((arg1),JAG_FILE,__LINE__)
#define vhGetDecLitValue(arg1)  VhGetDecLitValue((arg1),JAG_FILE,__LINE__)
#define vhIsDecLitPositive(arg1)  VhIsDecLitPositive((arg1),JAG_FILE,__LINE__)
#define vhIsReal(arg1)  VhIsReal((arg1),JAG_FILE,__LINE__)
#define vhIsExponent(arg1)  VhIsExponent((arg1),JAG_FILE,__LINE__)
#define vhGetIndexSubTypeDefList(arg1)  VhGetIndexSubTypeDefList((arg1),JAG_FILE,__LINE__)
#define vhGetDiscRangeList(arg1)  VhGetDiscRangeList((arg1),JAG_FILE,__LINE__)
#define vhGetDiscRangeListFromGenType(arg1)  VhGetDiscRangeListFromGenType((arg1),JAG_FILE,__LINE__)
#define vhGetGenMappedSubTypeInd(arg1)  VhGetGenMappedSubTypeInd((arg1),JAG_FILE,__LINE__)
#define vhGetArrayElementConstraint(arg1)  VhGetArrayElementConstraint((arg1),JAG_FILE,__LINE__)
#define vhGetArrayRootConstraint(arg1)  VhGetArrayRootConstraint((arg1),JAG_FILE,__LINE__)
#define vhGetIndexConstr(arg1)  VhGetIndexConstr((arg1),JAG_FILE,__LINE__)
#define vhGetIndexConstraint(arg1)  VhGetIndexConstr((arg1),JAG_FILE,__LINE__)
#define vhGetTypeType(arg1)  VhGetTypeType((arg1),JAG_FILE,__LINE__)
#define vhGetNoOfDim(arg1)  VhGetNoOfDim((arg1),JAG_FILE,__LINE__)
#define vhGetSigName(arg1)  VhGetSigName((arg1),JAG_FILE,__LINE__)
#define vhGetConstName(arg1)  VhGetConstName((arg1),JAG_FILE,__LINE__)
#define vhGetVarName(arg1)  VhGetVarName((arg1),JAG_FILE,__LINE__)
#define vhGetSubTypeInd(arg1)  VhGetSubTypeInd((arg1),JAG_FILE,__LINE__)
#define vhGetInitialValue(arg1)  VhGetInitialValue((arg1),JAG_FILE,__LINE__)
#define vhGetDefaultValueForObjDecl(arg1)  VhGetDefaultValueForObjDecl((arg1),JAG_FILE,__LINE__)
#define vhGetDeferredConstantValue(arg1)  VhGetDeferredConstantValue((arg1),JAG_FILE,__LINE__)
#define vhSetInitialValue(arg1,arg2)  VhSetInitialValue((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSetLineNumber(arg1,arg2)  VhSetLineNumber((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSetEndLineNumber(arg1,arg2)  VhSetEndLineNumber((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsAliasObject(arg1)  VhIsAliasObject((arg1),JAG_FILE,__LINE__)
#define vhGetActualForAlias(arg1)  VhGetActualForAlias((arg1),JAG_FILE,__LINE__)
#define vhGetAliasDeclFromObj(arg1)  VhGetAliasDeclFromObj((arg1),JAG_FILE,__LINE__)
#define vhGetObjectAliasDeclaration(arg1,arg2)  VhGetObjectAliasDeclaration((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsExplicitDecl(arg1)  VhIsExplicitDecl((arg1),JAG_FILE,__LINE__)
#define vhIsAssociated(arg1)  VhIsAssociated((arg1),JAG_FILE,__LINE__)
#define vhGetPortType(arg1)  VhGetPortType((arg1),JAG_FILE,__LINE__)
#define vhGetDisconnectSigKind(arg1)  VhGetDisconnectSigKind((arg1),JAG_FILE,__LINE__)
#define vhGetAttribSpecEntityNameListType(arg1)  VhGetAttribSpecEntityNameListType((arg1),JAG_FILE,__LINE__)
#define vhGetSigKind(arg1)  VhGetSigKind((arg1),JAG_FILE,__LINE__)
#define vhGetDisconnectTime(arg1)  VhGetDisconnectTime((arg1),JAG_FILE,__LINE__)
#define vhIsDeferred(arg1)  VhIsDeferred((arg1),JAG_FILE,__LINE__)
#define vhIsGeneric(arg1)  VhIsGeneric((arg1),JAG_FILE,__LINE__)
#define vhIsSubProgInterfaceConstant(arg1) VhIsSubProgInterfaceConstant((arg1),JAG_FILE,__LINE__)
#define vhIsSharedVar(arg1)  VhIsSharedVar((arg1),JAG_FILE,__LINE__)
#define vhGetFileName(arg1)  VhGetFileName((arg1),JAG_FILE,__LINE__)
#define vhGetFileOpenKind(arg1)  VhGetFileOpenKind((arg1),JAG_FILE,__LINE__)
#define vhGetFileOpenMode_87(arg1)  VhGetFileOpenMode_87((arg1),JAG_FILE,__LINE__)
#define vhGetFileLogicalName(arg1)  VhGetFileLogicalName((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgName(arg1)  VhGetSubProgName((arg1),JAG_FILE,__LINE__)
#define vhGetProtectedDecl(arg1)  VhGetProtectedDecl((arg1),JAG_FILE,__LINE__)
#define vhGetProtectedBody(arg1)  VhGetProtectedBody((arg1),JAG_FILE,__LINE__)
#define vhGetProtectedName(arg1)  VhGetProtectedName((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgKind(arg1)  VhGetSubProgKind((arg1),JAG_FILE,__LINE__)
#define vhGetPureType(arg1)  VhGetPureType((arg1),JAG_FILE,__LINE__)
#define vhIsOverloadedOp(arg1)  VhIsOverloadedOp((arg1),JAG_FILE,__LINE__)
#define vhGetReturnType(arg1)  VhGetReturnType((arg1),JAG_FILE,__LINE__)
#define vhGetReturnList(arg1)  VhGetReturnList((arg1),JAG_FILE,__LINE__)
#define vhGetFlatParamList(arg1)  VhGetFlatParamList((arg1),JAG_FILE,__LINE__)
#define vhGetBodyFlatParamList(arg1)  VhGetBodyFlatParamList((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgSpec(arg1)  VhGetSubProgSpec((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgBody(arg1)  VhGetSubProgBody((arg1),JAG_FILE,__LINE__)
#define vhSubProgHasUnconstrainedParam(arg1)  VhSubProgHasUnconstrainedParam((arg1),JAG_FILE,__LINE__)
#define vhSubprogUsesGlobals(arg1)  VhSubprogUsesGlobals((arg1),JAG_FILE,__LINE__)
#define vhGetGlobalsUsedInSubprog(arg1)  VhGetGlobalsUsedInSubprog((arg1),JAG_FILE,__LINE__)
#define vhGetConstantsUsedInSubprog(arg1)  VhGetConstantsUsedInSubprog((arg1),JAG_FILE,__LINE__)
#define vhGetSubprogsUsedInSubprog(arg1)  VhGetSubprogsUsedInSubprog((arg1),JAG_FILE,__LINE__)
#define vhIsSubProgRecursive(arg1)  VhIsSubProgRecursive((arg1),JAG_FILE,__LINE__)
#define vhIsSubProgOverloaded(arg1)  VhIsSubProgOverloaded((arg1),JAG_FILE,__LINE__)
#define vhIsScopeSynthesizable(arg1)  VhIsScopeSynthesizable((arg1),JAG_FILE,__LINE__)
#define vhHasNonSynthProperty(arg1,arg2)  VhHasNonSynthProperty((arg1),(arg2),JAG_FILE,__LINE__)
#define vhAllowOutsideReferenceInPureFunction(arg1)  VhAllowOutsideReferenceInPureFunction((arg1))

#define vhGetExpressionType(arg1)  VhGetExpressionType((arg1),JAG_FILE,__LINE__)
#define vhGetStaticType(arg1)  VhGetStaticType((arg1),JAG_FILE,__LINE__)
#define vhGetStaticValue(arg1)  VhGetStaticValue((arg1),JAG_FILE,__LINE__)
#define vhGetExprSize(arg1)  VhGetExprSize((arg1),JAG_FILE,__LINE__)
#define vhGetOpCat(arg1)  VhGetOpCat((arg1),JAG_FILE,__LINE__)
#define vhGetOpType(arg1)  VhGetOpType((arg1),JAG_FILE,__LINE__)
#define vhGetOperand(arg1)  VhGetOperand((arg1),JAG_FILE,__LINE__)
#define vhGetRightOperand(arg1)  VhGetRightOperand((arg1),JAG_FILE,__LINE__)
#define vhGetLeftOperand(arg1)  VhGetLeftOperand((arg1),JAG_FILE,__LINE__)
#define vhGetFuncCall(arg1)  VhGetFuncCall((arg1),JAG_FILE,__LINE__)
#define vhGetUninstSubProgName(arg1)  VhGetUninstSubProgName((arg1),JAG_FILE,__LINE__)
#define vhGetMaster(arg1)  VhGetMaster((arg1),JAG_FILE,__LINE__)
#define vhIsImplicitFuncCall(arg1)  VhIsImplicitFuncCall((arg1),JAG_FILE,__LINE__)
#define vhGetAssocList(arg1)  VhGetAssocList((arg1),JAG_FILE,__LINE__)
#define vhGetPrefix(arg1)  VhGetPrefix((arg1),JAG_FILE,__LINE__)
#define vhGetExprList(arg1)  VhGetExprList((arg1),JAG_FILE,__LINE__)
#define vhGetEleAssocList(arg1)  VhGetEleAssocList((arg1),JAG_FILE,__LINE__)
#define vhGetChoiceList(arg1)  VhGetChoiceList((arg1),JAG_FILE,__LINE__)
#define vhGetAllocOperand(arg1)  VhGetAllocOperand((arg1),JAG_FILE,__LINE__)
#define vhGetSimpleName(arg1)  VhGetSimpleName((arg1),JAG_FILE,__LINE__)
#define vhGetSimpleNameType(arg1)  VhGetSimpleNameType((arg1),JAG_FILE,__LINE__)
#define vhGetExpr(arg1)  VhGetExpr((arg1),JAG_FILE,__LINE__)
#define vhGetSelDeclExpr(arg1)  VhGetSelDeclExpr((arg1),JAG_FILE,__LINE__)
#define vhGetPortClause(arg1)  VhGetPortClause((arg1),JAG_FILE,__LINE__)
#define vhGetGenericClause(arg1)  VhGetGenericClause((arg1),JAG_FILE,__LINE__)
#define vhGetFlatGenericList(arg1)  VhGetFlatGenericList((arg1),JAG_FILE,__LINE__)
#define vhGetFlatSignalList(arg1)  VhGetFlatSignalList((arg1),JAG_FILE,__LINE__)
#define vhIsNamedAssociation(arg1)  VhIsNamedAssociation((arg1),JAG_FILE,__LINE__)
#define vhPortIsPurelyUnconstraint(arg1)  VhPortIsPurelyUnconstraint((arg1),JAG_FILE,__LINE__)
#define vhPortIsGenericDependent(arg1)  VhPortIsGenericDependent((arg1),JAG_FILE,__LINE__)
#define vhGetCompName(arg1)  VhGetCompName((arg1),JAG_FILE,__LINE__)
#define vhGetPortMap(arg1)  VhGetPortMap((arg1),JAG_FILE,__LINE__)
#define vhGetGenericMap(arg1)  VhGetGenericMap((arg1),JAG_FILE,__LINE__)
#define vhGetFormal(arg1)  VhGetFormal((arg1),JAG_FILE,__LINE__)
#define vhGetActual(arg1)  VhGetActual((arg1),JAG_FILE,__LINE__)
#define vhGetSliceSize(arg1)  VhGetSliceSize((arg1),JAG_FILE,__LINE__)
#define vhGetPortMapAspect(arg1)  VhGetPortMapAspect((arg1),JAG_FILE,__LINE__)
#define vhGetGenericMapAspect(arg1)  VhGetGenericMapAspect((arg1),JAG_FILE,__LINE__)
#define vhIsGuarded(arg1)  VhIsGuarded((arg1),JAG_FILE,__LINE__)
#define vhGetGuardSignal(arg1)  VhGetGuardSignal((arg1),JAG_FILE,__LINE__)
#define vhGetGuardExpr(arg1)  VhGetGuardExpr((arg1),JAG_FILE,__LINE__)
#define vhGetLabel(arg1)  VhGetLabel((arg1),JAG_FILE,__LINE__)
#define vhGetLabelObj(arg1)  VhGetLabelObj((arg1),JAG_FILE,__LINE__)
#define vhIsPostponed(arg1)  VhIsPostponed((arg1),JAG_FILE,__LINE__)
#define vhGetConcStmtList(arg1)  VhGetConcStmtList((arg1),JAG_FILE,__LINE__)
#define vhGetConcStmtListLength(arg1)  VhGetConcStmtListLength((arg1),JAG_FILE,__LINE__)
#define vhCreateExprFromString(arg1, arg2)  VhCreateExprFromString((arg1),(arg2),JAG_FILE,__LINE__)
#define vhEvaluateConstantExpr(arg1)  VhEvaluateConstantExpr((arg1),JAG_FILE,__LINE__)
#define vhGetAlternativeLabel(arg1)  VhGetAlternativeLabel((arg1),JAG_FILE,__LINE__)
#define vhGetElsIfGenerateList(arg1)  VhGetElsIfGenerateList((arg1),JAG_FILE,__LINE__)
#define vhGetParamSpec(arg1)  VhGetParamSpec((arg1),JAG_FILE,__LINE__)
#define vhGetElseGenerateNode(arg1)  VhGetElseGenerateNode((arg1),JAG_FILE,__LINE__)
#define vhGetParamSpecId(arg1)  VhGetParamSpecId((arg1),JAG_FILE,__LINE__)
#define vhGetTarget(arg1)  VhGetTarget((arg1),JAG_FILE,__LINE__)
#define vhGetWaveFrm(arg1)  VhGetWaveFrm((arg1),JAG_FILE,__LINE__)
#define vhGetWaveFrmEleList(arg1)  VhGetWaveFrmEleList((arg1),JAG_FILE,__LINE__)
#define vhGetValueExpr(arg1)  VhGetValueExpr((arg1),JAG_FILE,__LINE__)
#define vhGetTimeExpr(arg1)  VhGetTimeExpr((arg1),JAG_FILE,__LINE__)
#define vhGetDelayMech(arg1)  VhGetDelayMech((arg1),JAG_FILE,__LINE__)
#define vhIsUnaffected(arg1)  VhIsUnaffected((arg1),JAG_FILE,__LINE__)
#define vhIsNullTransaction(arg1)  VhIsNullTransaction((arg1),JAG_FILE,__LINE__)
#define vhIsTransport(arg1)  VhIsTransport((arg1),JAG_FILE,__LINE__)
#define vhIsReject(arg1)  VhIsReject((arg1),JAG_FILE,__LINE__)
#define vhIsInertial(arg1)  VhIsInertial((arg1),JAG_FILE,__LINE__)
#define vhGetSelExpr(arg1)  VhGetSelExpr((arg1),JAG_FILE,__LINE__)
#define vhGetSelWaveFrmList(arg1)  VhGetSelWaveFrmList((arg1),JAG_FILE,__LINE__)
#define vhGetConditionalWaveFrmList(arg1)  VhGetConditionalWaveFrmList((arg1),JAG_FILE,__LINE__)
#define vhGetAssertion(arg1)  VhGetAssertion((arg1),JAG_FILE,__LINE__)
#define vhGetProcedureCall(arg1)  VhGetProcedureCall((arg1),JAG_FILE,__LINE__)
#define vhGetSensitivityList(arg1)  VhGetSensitivityList((arg1),JAG_FILE,__LINE__)
#define vhGetConditionClause(arg1)  VhGetConditionClause((arg1),JAG_FILE,__LINE__)
#define vhGetTimeoutClause(arg1)  VhGetTimeoutClause((arg1),JAG_FILE,__LINE__)
#define vhIsSensitive(arg1)  VhIsSensitive((arg1),JAG_FILE,__LINE__)
#define vhGetElseCondWaveFrm(arg1)  VhGetElseCondWaveFrm((arg1),JAG_FILE,__LINE__)
#define vhHasWait(arg1)  VhHasWait((arg1),JAG_FILE,__LINE__)
#define vhHasSignalAsgn(arg1)  VhHasSignalAsgn((arg1),JAG_FILE,__LINE__)
#define vhGetImplicitSensList(arg1)  VhGetImplicitSensList((arg1),JAG_FILE,__LINE__)
#define vhGetResolutionMethod(arg1)  VhGetResolutionMethod((arg1),JAG_FILE,__LINE__)
#define vhGetDriversList(arg1)  VhGetDriversList((arg1),JAG_FILE,__LINE__)
#define vhGetProcessClock(arg1)  VhGetProcessClock((arg1),JAG_FILE,__LINE__)
#define vhGetProcessClockExpr(arg1)  VhGetProcessClockExpr((arg1),JAG_FILE,__LINE__)
#define vhGetProcessClockEdge(arg1)  VhGetProcessClockEdge((arg1),JAG_FILE,__LINE__)
#define vhGetProcessAsyncSignalsList(arg1)  VhGetProcessAsyncSignalsList((arg1),JAG_FILE,__LINE__)
#define vhGetDriverSignal(arg1)  VhGetDriverSignal((arg1),JAG_FILE,__LINE__)
#define vhGetDriverStatement(arg1)  VhGetDriverStatement((arg1),JAG_FILE,__LINE__)
#define vhGetReadOnlySignals(arg1)  vhGetReadOnlySignalsInProcess((arg1))
#define vhGetWriteOnlySignals(arg1)  vhGetWriteOnlySignalsInProcess((arg1))
#define vhGetReadWriteSignals(arg1)  vhGetReadWriteSignalsInProcess((arg1))
#define vhGetReadOnlyObjectList(arg1)  VhGetReadOnlyObjectList((arg1),JAG_FILE,__LINE__)
#define vhGetSource(arg1)  VhGetSource((arg1),JAG_FILE,__LINE__)
#define vhGetProcedureName(arg1)  VhGetProcedureName((arg1),JAG_FILE,__LINE__)
#define vhGetCondition(arg1)  VhGetCondition((arg1),JAG_FILE,__LINE__)
#define vhGetThenSeqStmtList(arg1)  VhGetThenSeqStmtList((arg1),JAG_FILE,__LINE__)
#define vhGetElsifSeqStmtList(arg1)  VhGetElsifSeqStmtList((arg1),JAG_FILE,__LINE__)
#define vhGetElsSeqStmtList(arg1)  VhGetElsSeqStmtList((arg1),JAG_FILE,__LINE__)
#define vhGetElseStmtNode(arg1)  VhGetElseStmtNode((arg1),JAG_FILE,__LINE__)
#define vhGetCaseExpr(arg1)  VhGetCaseExpr((arg1),JAG_FILE,__LINE__)
#define vhGetAlternativeList(arg1)  VhGetAlternativeList((arg1),JAG_FILE,__LINE__)
#define vhIsOthers(arg1)  VhIsOthers((arg1),JAG_FILE,__LINE__)
#define vhGetLoopStmt(arg1)  VhGetLoopStmt((arg1),JAG_FILE,__LINE__)
#define vhGetEndLoopStmt(arg1)  VhGetEndLoopStmt((arg1),JAG_FILE,__LINE__)
#define vhGetLoopLabel(arg1)  VhGetLoopLabel((arg1),JAG_FILE,__LINE__)
#define vhGetSeqStmtList(arg1)  VhGetSeqStmtList((arg1),JAG_FILE,__LINE__)
#define vhGetReportExpr(arg1)  VhGetReportExpr((arg1),JAG_FILE,__LINE__)
#define vhGetSeverityExpr(arg1)  VhGetSeverityExpr((arg1),JAG_FILE,__LINE__)
#define vhGetNextLabelObj(arg1)  VhGetNextLabelObj((arg1),JAG_FILE,__LINE__)
#define vhGetExitLabelObj(arg1)  VhGetExitLabelObj((arg1),JAG_FILE,__LINE__)
#define vhGetAffectedLoop(arg1)  VhGetAffectedLoop((arg1),JAG_FILE,__LINE__)
#define vhGetAttribName(arg1)  VhGetAttribName((arg1),JAG_FILE,__LINE__)
#define vhGetEntityClass(arg1)  VhGetEntityClass((arg1),JAG_FILE,__LINE__)
#define vhGetEntityDesigList(arg1)  VhGetEntityDesigList((arg1),JAG_FILE,__LINE__)
#define vhGetEntityTag(arg1)  VhGetEntityTag((arg1),JAG_FILE,__LINE__)
#define vhGetAttribValue(arg1)  VhGetAttribValue((arg1),JAG_FILE,__LINE__)
#define vhGetAttribDecl(arg1)  VhGetAttribDecl((arg1),JAG_FILE,__LINE__)
#define vhGetSignature(arg1)  VhGetSignature((arg1),JAG_FILE,__LINE__)
#define vhGetTypeMarkList(arg1)  VhGetTypeMarkList((arg1),JAG_FILE,__LINE__)
#define vhGetAttrb(arg1)  VhGetAttrb((arg1),JAG_FILE,__LINE__)
#define vhGetAttrbType(arg1)  VhGetAttrbType((arg1),JAG_FILE,__LINE__)
#define vhGetGroupTmplName(arg1)  VhGetGroupTmplName((arg1),JAG_FILE,__LINE__)
#define vhGetEntityClassList(arg1)  VhGetEntityClassList((arg1),JAG_FILE,__LINE__)
#define vhGetEntClassType(arg1)  VhGetEntClassType((arg1),JAG_FILE,__LINE__)
#define vhIsBoxAtEnd(arg1)  VhIsBoxAtEnd((arg1),JAG_FILE,__LINE__)
#define vhGetGroupDeclName(arg1)  VhGetGroupDeclName((arg1),JAG_FILE,__LINE__)
#define vhGetGroupDeclTemplate(arg1)  VhGetGroupDeclTemplate((arg1),JAG_FILE,__LINE__)
#define vhGetGroupConstituentList(arg1)  VhGetGroupConstituentList((arg1),JAG_FILE,__LINE__)
#define vhGetDisconnectSpecSignalList(arg1)  VhGetDisconnectSpecSignalList((arg1),JAG_FILE,__LINE__)
#define vhGetDisconnectSpecTime(arg1)  VhGetDisconnectSpecTime((arg1),JAG_FILE,__LINE__)
#define vhGetAliasName(arg1)  VhGetAliasName((arg1),JAG_FILE,__LINE__)
#define vhGetAliasDesig(arg1)  VhGetAliasDesig((arg1),JAG_FILE,__LINE__)
#define vhGetInstList(arg1)  VhGetInstList((arg1),JAG_FILE,__LINE__)
#define vhGetCompSpec(arg1)  VhGetCompSpec((arg1),JAG_FILE,__LINE__)
#define vhGetBindInd(arg1)  VhGetBindInd((arg1),JAG_FILE,__LINE__)
#define vhGetBlockConfig(arg1)  VhGetBlockConfig((arg1),JAG_FILE,__LINE__)
#define vhGetBlockSpec(arg1)  VhGetBlockSpec((arg1),JAG_FILE,__LINE__)
#define vhGetBlockConfigUniqueId(arg1)  VhGetBlockConfigUniqueId((arg1),JAG_FILE,__LINE__)
#define vhGetEntityAspect(arg1)  VhGetEntityAspect((arg1),JAG_FILE,__LINE__)
#define vhIsArchNameImplicitMRA(arg1)  VhIsArchNameImplicitMRA((arg1),JAG_FILE,__LINE__)
#define vhGetConfigItemList(arg1)  VhGetConfigItemList((arg1),JAG_FILE,__LINE__)
#define vhGetCompDecl(arg1)  VhGetCompDecl((arg1),JAG_FILE,__LINE__)
#define vhGetConfigSpec(arg1)  VhGetConfigSpec((arg1),JAG_FILE,__LINE__)
#define vhGetDefaultBinding(arg1)  VhGetDefaultBinding((arg1),JAG_FILE,__LINE__)
#define vhSetAttr(arg1,arg2,arg3)  VhSetAttr((arg1),(arg2),(vhString)(arg3),JAG_FILE,__LINE__)
#define vhGetAttr(arg1,arg2)  VhGetAttr((arg1),(vhString)(arg2),JAG_FILE,__LINE__)
#define vhDumpUserAttribute(arg1)  VhDumpUserAttribute((arg1),JAG_FILE,__LINE__)
#define vhGetDUEncryptionInfo(arg1)  VhGetDUEncryptionInfo((arg1),JAG_FILE,__LINE__)
#define vhStoreUserComponentName()  VhStoreUserComponentName(JAG_FILE,__LINE__)
#define vhStoreComponentFormalList() VhStoreComponentFormalList(JAG_FILE,__LINE__)
#define vhStoreElseLineNoOfIfStmnt()  VhStoreElseLineNoOfIfStmnt(JAG_FILE,__LINE__)
#define vhGetElseLineNoOfIfStmnt(arg1)  VhGetElseLineNoOfIfStmnt((arg1),JAG_FILE,__LINE__)
#define vhIsCompiledWith87Mode(arg1)  VhIsCompiledWith87Mode((arg1),JAG_FILE,__LINE__)
#define vhGetVHDLCompilationMode(arg1)  VhGetVHDLCompilationMode((arg1),JAG_FILE,__LINE__)
#define vhAttributeCreate(arg1,arg2,arg3,arg4)  VhAttributeCreate((arg1),(arg2),(arg3),(arg4),JAG_FILE,__LINE__)
#define vhNodeAttachAttribute(arg1,arg2,arg3)  VhNodeAttachAttribute((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhNodeGetAttributeValue(arg1,arg2)  VhNodeGetAttributeValue((arg1),(arg2),JAG_FILE,__LINE__)
#define vhLookUpInSymTab(arg1,arg2)  VhLookUpInSymTab((arg1),(arg2),JAG_FILE,__LINE__)
#define vhStopAtVhExternalNode(arg1)  VhStopAtVhExternalNode((arg1),JAG_FILE,__LINE__)
#define vhIsA(arg1,arg2)  VhIsA((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetObjTypeName(arg1)  VhGetObjTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetBaseObjType(arg1)  VhGetBaseObjType((arg1),JAG_FILE,__LINE__)
#define vhGetDerivedTypes(arg1)  VhGetDerivedTypes((arg1),JAG_FILE,__LINE__)
#define vhGetScope(arg1)  VhGetScope((arg1),JAG_FILE,__LINE__)
#define vhGetMasterScope(arg1)  VhGetMasterScope((arg1),JAG_FILE,__LINE__)
#define vhGetActualObj(arg1)  VhGetActualObj((arg1),JAG_FILE,__LINE__)
#define vhGetActualObjType(arg1)  VhGetActualObjType((arg1),JAG_FILE,__LINE__)
#define vhGetActualObjScopeType(arg1)  VhGetActualObjScopeType((arg1),JAG_FILE,__LINE__)
#define vhGetActualObjScopeName(arg1)  VhGetActualObjScopeName((arg1),JAG_FILE,__LINE__)
#define vhGetPrimUnitName(arg1)  VhGetPrimUnitName((arg1),JAG_FILE,__LINE__)
#define vhGetSynthesizedResFunction(arg1,arg2) VhGetSynthesizedResFunction((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSimulatorCompatible(arg1) VhSimulatorCompatible((arg1),JAG_FILE,__LINE__)
#define vhVcsCompatible(arg1) VhSimulatorCompatible((arg1),JAG_FILE,__LINE__)
#define vhAllowGenerateStmtWithoutBeginKeyword(arg1) VhAllowGenerateStmtWithoutBeginKeyword((arg1),JAG_FILE,__LINE__)
#define vhAllowDefactoStdSyntax(arg1) VhAllowDefactoStdSyntax((arg1),JAG_FILE,__LINE__)
#define vhMentorCompatible(arg1) VhMentorCompatible((arg1),JAG_FILE,__LINE__)
#define vhDetectClock(arg1,arg2) VhDetectClock((arg1),(arg2),JAG_FILE,__LINE__)
#define vhDetectClockInWaitStmt(arg1,arg2) VhDetectClockInWaitStmt((arg1),(arg2),JAG_FILE,__LINE__)
#define vhStoreInitialValues(arg1) VhStoreInitialValues((arg1),JAG_FILE,__LINE__)
#define vhPropagateValue_new(arg1,arg2) VhPropagateValue_new((arg1),(arg2),JAG_FILE,__LINE__)
#define vhPrintEEAddress(arg1) VhPrintEEAddress((arg1))
#define vhPropagateValue(arg1,arg2) VhPropagateValue((arg1),(arg2),JAG_FILE,__LINE__)
#define vhPropagateValueVer2(arg1,arg2) VhPropagateValueVer2((arg1),(arg2),JAG_FILE,__LINE__)
#define vhReturnPartialStaticValue(arg1) VhReturnPartialStaticValue((arg1),JAG_FILE,__LINE__)
#define vhGetStatusOfEvalParialStatic()   VhGetStatusOfEvalParialStatic(JAG_FILE,__LINE__)
#define vhGetStatusOfIsPartValSupported() VhGetStatusOfIsPartValSupported(JAG_FILE,__LINE__)
#define vhResolveAliasExternal(arg1)      VhResolveAliasExternal((arg1),JAG_FILE,__LINE__)
#define vhPrintAllGlobals(arg1)           VhPrintAllGlobals((arg1),JAG_FILE,__LINE__)
#define vhwCreateNonStaticValue(arg1) VhwCreateNonStaticValue((arg1),JAG_FILE,__LINE__)
#define vhDetectGatedClock(arg1) VhDetectGatedClock((arg1),JAG_FILE,__LINE__)
#define vhSupportEnumTypeGeneric(arg1) VhSupportEnumTypeGeneric((arg1),JAG_FILE,__LINE__)
#define vhLibraryPathCompatible(arg1) VhLibraryPathCompatible((arg1),JAG_FILE,__LINE__)
#define vhGetVersionInfo() VhGetVersionInfo(JAG_FILE,__LINE__)
#define vhGetAllPrimaryUnits(arg1)  VhGetAllPrimaryUnits((arg1),JAG_FILE,__LINE__)
#define vhGetAllSecondaryUnits(arg1,arg2)  VhGetAllSecondaryUnits((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetAllEntities(arg1)  VhGetAllEntities((arg1),JAG_FILE,__LINE__)
#define vhGetAllPackages(arg1)  VhGetAllPackages((arg1),JAG_FILE,__LINE__)
#define vhGetAllConfigurations(arg1)  VhGetAllConfigurations((arg1),JAG_FILE,__LINE__)
#define vhGetAllContextDeclarations(arg1)  VhGetAllContextDeclarations((arg1),JAG_FILE,__LINE__)
#define vhGetAllArchitectures(arg1,arg2)  VhGetAllArchitectures((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetLibPathName(arg1)  VhGetLibPathName((vhString)(arg1),JAG_FILE,__LINE__)
#define vhGetPhysicalPathFromLogicalName(arg1)  VhGetPhysicalPathFromLogicalName((arg1),JAG_FILE,__LINE__)
#define vhIsUnitVerifUnit(arg1,arg2)  VhIsUnitVerifUnit((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsUnitEntity(arg1,arg2)  VhIsUnitEntity((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsUnitPackage(arg1,arg2)  VhIsUnitPackage((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsUnitConfiguration(arg1,arg2)  VhIsUnitConfiguration((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetDeclItemList(arg1)  VhGetDeclItemList((arg1),JAG_FILE,__LINE__)
#define vhGetDeclListLength(arg1)  VhGetDeclListLength((arg1),JAG_FILE,__LINE__)
#define vhGetName(arg1)  VhGetName((arg1),JAG_FILE,__LINE__)
#define vhGetInterfaceList(arg1)  VhGetInterfaceList((arg1),JAG_FILE,__LINE__)
#define vhGetNonFlatInterfaceList(arg1)  VhGetNonFlatInterfaceList((arg1),JAG_FILE,__LINE__)
#define vhGetExternalNameList(arg1)  VhGetExternalNameList((arg1),JAG_FILE,__LINE__)
#define vhGetPathNameListOfExtName(arg1)  VhGetPathNameListOfExtName((arg1),JAG_FILE,__LINE__)
#define vhGetUsedAsExtNameListInCurrScope(arg1)  VhGetUsedAsExtNameListInCurrScope((arg1),JAG_FILE,__LINE__)
#define vhResolveExternalName(arg1,arg2)  VhResolveExternalName((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSetActualOfExtName(arg1,arg2,arg3)  VhSetActualOfExtName((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhSetInstanceMaster(arg1,arg2)  VhSetInstanceMaster((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetInstanceMaster(arg1)  VhGetInstanceMaster((arg1),JAG_FILE,__LINE__)
#define vhGetExtNamePathNameCategory(arg1)  VhGetExtNamePathNameCategory((arg1),JAG_FILE,__LINE__)
#define vhExtNameAddNodeToObjectList(arg1,arg2)  VhExtNameAddNodeToObjectList((arg1),(arg2),JAG_FILE,__LINE__)
#define vhExtNameGetObjectList(arg1)  VhExtNameGetObjectList((arg1),JAG_FILE,__LINE__)
#define vhExtNameGetUpwardRefCount(arg1)  VhExtNameGetUpwardRefCount((arg1),JAG_FILE,__LINE__)
#define vhExtNameSetActualInMixed(arg1,arg2)  VhExtNameSetActualInMixed((arg1),(arg2),JAG_FILE,__LINE__)
#define vhExtNameGetActualInMixed(arg1)  VhExtNameGetActualInMixed((arg1),JAG_FILE,__LINE__)
#define vhGetExtNameClassType(arg1)  VhGetExtNameClassType((arg1),JAG_FILE,__LINE__)
#define vhGetExtNameObjName(arg1)  VhGetExtNameObjName((arg1),JAG_FILE,__LINE__)
#define vhExtNamePopulateTypeInfo(arg1) VhExtNamePopulateTypeInfo((arg1),JAG_FILE,__LINE__)
#define vhSetExtNameStaticValue(arg1, arg2) VhSetExtNameStaticValue((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetExtNameElabOnly(arg1) VhGetExtNameElabOnly((arg1),JAG_FILE,__LINE__)
#define vhSetExtNameLastEntityName(arg1, arg2) VhSetExtNameLastEntityName((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSetExtNameParentNetName(arg1, arg2) VhSetExtNameParentNetName((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetExtNameLastEntityName(arg1) VhGetExtNameLastEntityName((arg1),JAG_FILE,__LINE__)
#define vhGetExtNameParentNetName(arg1) VhGetExtNameParentNetName((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgDeclList(arg1)  VhGetSubProgDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgBodyList(arg1)  VhGetSubProgBodyList((arg1),JAG_FILE,__LINE__)
#define vhGetFunctionDeclList(arg1)  VhGetFunctionDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetProcedureDeclList(arg1)  VhGetProcedureDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetTypeDeclList(arg1)  VhGetTypeDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetSubTypeDeclList(arg1)  VhGetSubTypeDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetConfigSpecList(arg1)  VhGetConfigSpecList((arg1),JAG_FILE,__LINE__)
#define vhGetCompDeclList(arg1)  VhGetCompDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetSignalDeclList(arg1)  VhGetSignalDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetConstDeclList(arg1)  VhGetConstDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetVarDeclList(arg1)  VhGetVarDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetAttrbDeclList(arg1)  VhGetAttrbDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetAttrbSpecList(arg1)  VhGetAttrbSpecList((arg1),JAG_FILE,__LINE__)
#define vhGetEnumDeclList(arg1)  VhGetEnumDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetComponentDeclList(arg1)  VhGetComponentDeclList((arg1),JAG_FILE,__LINE__)
#define vhGetCompInstList(arg1)  VhGetCompInstList((arg1),JAG_FILE,__LINE__)
#define vhGetContextItemList(arg1)  VhGetContextItemList((arg1),JAG_FILE,__LINE__)
#define vhGetContextDeclItemList(arg1)  VhGetContextDeclItemList((arg1),JAG_FILE,__LINE__)
#define vhGetActiveLowSignalList(arg1)  VhGetActiveLowSignalList((arg1),JAG_FILE,__LINE__)
#define vhGetOpTypeName(arg1)  VhGetOpTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetBooleanName(arg1)  VhGetBooleanName((arg1),JAG_FILE,__LINE__)
#define vhGetBaseTypeName(arg1)  VhGetBaseTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetDirTypeName(arg1)  VhGetDirTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetPortTypeName(arg1)  VhGetPortTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetSigKindName(arg1)  VhGetSigKindName((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgTypeName(arg1)  VhGetSubProgTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetAttrbTypeName(arg1)  VhGetAttrbTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetTypeTypeName(arg1)  VhGetTypeTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetStaticTypeName(arg1)  VhGetStaticTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetVhEntityClassName(arg1)  VhGetVhEntityClassName((arg1),JAG_FILE,__LINE__)
#define vhGetPureTypeName(arg1)  VhGetPureTypeName((arg1),JAG_FILE,__LINE__)
#define vhGetOpCategoryName(arg1)  VhGetOpCategoryName((arg1),JAG_FILE,__LINE__)
#define vhGetResolutionMethodName(arg1)  VhGetResolutionMethodName((arg1),JAG_FILE,__LINE__)
#define vhElaborateSizeNode(arg1)  VhElaborateSizeNode((arg1),JAG_FILE,__LINE__)
#define vhDeleteEEAddress(arg1) VhDeleteEEAddress((arg1),JAG_FILE,__LINE__)
#define vhEvaluateToEEAddress(arg1)  VhEvaluateToEEAddress((arg1),JAG_FILE,__LINE__)
#define vhGetExprFromEEAddress(arg1,arg2,arg3)  VhGetExprFromEEAddress((arg1),(arg2),(arg3), JAG_FILE,__LINE__)
#define vhUpdateGenericsInEntityContainer(arg1)  VhUpdateGenericsInEntityContainer((arg1),JAG_FILE,__LINE__)
#define vhEvaluateExpr(arg1)  VhEvaluateExpr((arg1),JAG_FILE,__LINE__)
#define vhCustomEvaluateExpr(arg1,arg2) VhCustomEvaluateExpr((arg1),(arg2),JAG_FILE,__LINE__)
#define vhOpenDeclarativeRegionNoFolding(arg1,arg2) VhOpenDeclarativeRegionNoFolding((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSetNonStatArgExprEval(arg1)  VhSetNonStatArgExprEval((arg1),JAG_FILE,__LINE__)
#define vhStopConstPropagationInFuncEval(arg1)  VhStopConstPropagationInFuncEval((arg1),JAG_FILE,__LINE__)
#define vhKeepSubTypeIndOfTypeDeclInElab(arg1)  VhKeepSubTypeIndOfTypeDeclInElab((arg1),JAG_FILE,__LINE__)
#define vhCheckBindingIndInMixFlow(arg1)  VhCheckBindingIndInMixFlow((arg1),JAG_FILE,__LINE__)
#define vhDontElabNonStatExprSizes(arg1)  VhDontElabNonStatExprSizes((arg1),JAG_FILE,__LINE__)
#define vhEvaluateOperator(arg1,arg2,arg3) VhEvaluateOperator((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhEvaluatePartialStatic(arg1) VhEvaluatePartialStatic((arg1),JAG_FILE,__LINE__)
#define vhStoreDynamicValue(arg1,arg2) VhStoreDynamicValue((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetDynamicValue(arg1) VhGetDynamicValue((arg1),JAG_FILE,__LINE__)
#define vhSetRecursiveEvaluationDepth(arg1) VhSetRecursiveEvaluationDepth((arg1),JAG_FILE,__LINE__)
#define vhPutDefaultInitialValue(arg1) VhPutDefaultInitialValue((arg1),JAG_FILE,__LINE__)
#define vhGetElabEntity(arg1)  VhGetElabEntity((arg1),JAG_FILE,__LINE__)
#define vhAllowPragmaOnChainDecl(arg1)  VhAllowPragmaOnChainDecl((arg1),JAG_FILE,__LINE__)
#define vhIsCallBackEntity(arg1)  VhIsCallBackEntity((arg1),JAG_FILE,__LINE__)
#define vhAllowEntityWithOutArchInElab(arg1)  VhAllowEntityWithOutArchInElab((arg1),JAG_FILE,__LINE__)
#define vhGetChainDeclFromDirective(arg1)  VhGetChainDeclFromDirective((arg1),JAG_FILE,__LINE__)
#define vhGetBlockCommentFromRegionMarker(arg1)  VhGetBlockCommentFromRegionMarker((arg1),JAG_FILE,__LINE__)
#define vhCreateRegionMarkerWithBlockComment(arg1,arg2,arg3)  VhCreateRegionMarkerWithBlockComment((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhOpenInstance(arg1,arg2)  VhOpenInstance((arg1),(arg2),JAG_FILE,__LINE__)
#define vhElaborateBlackBox(arg1,arg2)  VhElaborateBlackBox((arg1),(arg2),JAG_FILE,__LINE__)
#define vhElaborateStatement(arg1,arg2)  VhElaborateStatement((arg1),(arg2),JAG_FILE,__LINE__)
#define vhStatementNeedsElaboration(arg1)  VhStatementNeedsElaboration((arg1),JAG_FILE,__LINE__)
#define vhGetElabArch(arg1)  VhGetElabArch((arg1),JAG_FILE,__LINE__)
#define vhGetElabBlockConfig(arg1)  VhGetElabBlockConfig((arg1),JAG_FILE,__LINE__)
#define vhFindInBlockConfig(arg1,arg2)  VhFindInBlockConfig((arg1),(arg2),JAG_FILE,__LINE__)
#define vhReplaceOpensInActual(arg1,arg2)  VhReplaceOpensInActual((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetStatus(arg1)  VhGetStatus((arg1),JAG_FILE,__LINE__)
#define vhOpenDeclarativeRegion(arg1)  VhOpenDeclarativeRegion((arg1),JAG_FILE,__LINE__)
#define vhOpenDeclarativeRegionFuncCall(arg1)  VhOpenDeclarativeRegionFuncCall(arg1)
#define vhGetTypeInfo(arg1)  VhGetTypeInfo(arg1)
//#define vhCloseRegionFuncCall(arg1)  VhCloseRegionFuncCall(arg1)
#define vhUpdateContainer(arg1)  VhUpdateContainer(arg1)
#define vhUpdateAddrContainer(arg1) VhUpdateAddrContainer((arg1))
#define vhGetScopeContainsElabOnlyExtName(arg1) VhGetScopeContainsElabOnlyExtName((arg1))
//#define vhSetScopeContainsElabOnlyExtName(arg1,arg2) VhSetScopeContainsElabOnlyExtName((arg1),(arg2))
#define vhInitializeForIndexAddr(arg1,arg2) VhInitializeForIndexAddr((arg1),(arg2));
#define vhUpdateForIndexAddr(arg1,arg2) VhUpdateForIndexAddr((arg1),(arg2));
#define vhDeleteForIndexAddr(arg1) VhDeleteForIndexAddr((arg1))
#define vhInitializeForGenIndexAddr(arg1,arg2) VhInitializeForGenIndexAddr((arg1),(arg2));
#define vhUpdateForGenIndexAddr(arg1,arg2) VhUpdateForGenIndexAddr((arg1),(arg2));
#define vhDeleteForGenIndexAddr(arg1) VhDeleteForGenIndexAddr((arg1))
#define vhRestoreForGenIndex(arg1,arg2) VhRestoreForGenIndex((arg1),(arg2))
#define vhOpenBlockHeader(arg1)  VhOpenBlockHeader((arg1),JAG_FILE,__LINE__)
#define vhOpenSubProgram(arg1)  VhOpenSubProgram((arg1),JAG_FILE,__LINE__)
#define vhOpenOverloadedExpr(arg1)  VhOpenOverloadedExpr((arg1),JAG_FILE,__LINE__)
#define vhGetElaborateStatus()    VhGetElaborateStatus(JAG_FILE,__LINE__)
#define vhElaborateGenerateRange(arg1)  VhElaborateGenerateRange((arg1),JAG_FILE,__LINE__)
#define vhGetGenerateIndexValue(arg1)  VhGetGenerateIndexValue((arg1),JAG_FILE,__LINE__)
#define vhUpdateGenerateIndex(arg1)  VhUpdateGenerateIndex((arg1),JAG_FILE,__LINE__)
#define vhResetGenerateIndex(arg1)  VhResetGenerateIndex((arg1),JAG_FILE,__LINE__)

#define vhGetListofComments(arg1)  VhGetListofComments((arg1),JAG_FILE,__LINE__)
#define vhCommentGetString(arg1)  VhCommentGetString((arg1),JAG_FILE,__LINE__)
#define vhSetTranslateOnOffAsComment(arg1)  VhSetTranslateOnOffAsComment((arg1),JAG_FILE,__LINE__)
#define vhOverwriteTranslateOnOffAsComment(arg1)  VhOverwriteTranslateOnOffAsComment((arg1),JAG_FILE,__LINE__)
#define vhIgnoreTranslateOffOn(arg1)  VhIgnoreTranslateOffOn((arg1),JAG_FILE,__LINE__)
#define vhIgnoreSynthesisOffOn(arg1)  VhIgnoreSynthesisOffOn((arg1),JAG_FILE,__LINE__)
#define vhAllowPragmaUnderNoDumpSynth(arg1)  VhAllowPragmaUnderNoDumpSynth((arg1),JAG_FILE,__LINE__)
#define vhAllowNestedTranslateAndSynthesisPragma(arg1)  VhAllowNestedTranslateAndSynthesisPragma((arg1),JAG_FILE,__LINE__)
#define vhSetIgnoreSynthesisRegionDirectiveAsComment(arg1)  VhSetIgnoreSynthesisRegionDirectiveAsComment((arg1),JAG_FILE,__LINE__)
#define vhSetIgnoreSynthesisRegionDirectiveInsideExprAsComment(arg1)  VhSetIgnoreSynthesisRegionDirectiveInsideExprAsComment((arg1),JAG_FILE,__LINE__)
#define vhCompileFuncBodyWithinRegion(arg1)  VhCompileFuncBodyWithinRegion((arg1),JAG_FILE,__LINE__)
#define vhCompileFuncBodyWithinTransOffOnRegion(arg1)  VhCompileFuncBodyWithinTransOffOnRegion((arg1),JAG_FILE,__LINE__)

#define vhGetListofMetaComments(arg1)  VhGetListofMetaComments((arg1),JAG_FILE,__LINE__)
#define vhGetAllDirectives(arg1)  VhGetAllDirectives((arg1),JAG_FILE,__LINE__)
#define vhGetAssociatedObject(arg1)  VhGetAssociatedObject((arg1),JAG_FILE,__LINE__)
#define vhGetAssociatedDirectiveString(arg1)  VhGetAssociatedDirectiveString((arg1),JAG_FILE,__LINE__)
#define vhGetRegionMarkerList(arg1)  VhGetRegionMarkerList((arg1),JAG_FILE,__LINE__)
#define vhHasEndLabel(arg1) VhHasEndLabel((arg1),JAG_FILE,__LINE__)
#define vhIsObjectInRegion(arg1,arg2)  VhIsObjectInRegion((arg1),(arg2),JAG_FILE,__LINE__)
#define vhEnableDirctiveDecompile(arg1)  vhEnableDirectiveDecompile((arg1))
#define vhEnableMetaCommentsDecompile(arg1)  VhEnableMetaCommentsDecompile((arg1))
#define vhDisableDecompileWithinRegionMarker(arg1)  VhDisableDecompileWithinRegionMarker((arg1))
#define vhIsValidSynthesisRegion(arg1,arg2)  VhIsValidSynthesisRegion((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSetDisplayErrorInJPI(arg1)  VhSetDisplayErrorInJPI((arg1),JAG_FILE,__LINE__)
#define vhGetEntityDependencyList(arg1,arg2)  VhGetEntityDependencyList((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetVunitDependencyList(arg1,arg2)  VhGetVunitDependencyList((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetPackageDependencyList(arg1,arg2)  VhGetPackageDependencyList((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetConfigDependencyList(arg1,arg2)  VhGetConfigDependencyList((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetPackBodyDependencyList(arg1,arg2)  VhGetPackBodyDependencyList((arg1),(arg2),JAG_FILE,__LINE__)
#define vhGetArchDependencyList(arg1,arg2,arg3)  VhGetArchDependencyList((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhDependGetFileName(arg1)  VhDependGetFileName((arg1),JAG_FILE,__LINE__)
#define vhDependGetLibName(arg1)  VhDependGetLibName((arg1),JAG_FILE,__LINE__)
#define vhDependGetUnitName(arg1)  VhDependGetUnitName((arg1),JAG_FILE,__LINE__)
#define vhDependGetArchName(arg1)  VhDependGetArchName((arg1),JAG_FILE,__LINE__)
#define vhDependGetUnitType(arg1)  VhDependGetUnitType((arg1),JAG_FILE,__LINE__)
#define vhIsEntityUpToDate(arg1,arg2)  VhIsEntityUpToDate((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsArchUpToDate(arg1,arg2,arg3)  VhIsArchUpToDate((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhIsConfigUpToDate(arg1,arg2)  VhIsConfigUpToDate((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsPackDeclUpToDate(arg1,arg2)  VhIsPackDeclUpToDate((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsPackBodyUpToDate(arg1,arg2)  VhIsPackBodyUpToDate((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsUnInstPack(arg1)  VhIsUnInstPack((arg1),JAG_FILE,__LINE__)
#define vhIsFuncCallArgCreatedFromDefaultVal(arg1)  VhIsFuncCallArgCreatedFromDefaultVal((arg1),JAG_FILE,__LINE__)
#define vhJaguarAnalyze(arg1,arg2)  VhJaguarAnalyze((arg1),(arg2),JAG_FILE,__LINE__)
#define vhRangeCheckOnExpression(arg1)  VhRangeCheckOnExpression((arg1),JAG_FILE,__LINE__)
#define vhRangeCheckOnStatement(arg1)  VhRangeCheckOnStatement((arg1),JAG_FILE,__LINE__)
#define vhRangeCheckOnRegion(arg1)  VhRangeCheckOnRegion((arg1),JAG_FILE,__LINE__)
#define vhCreateFaninFanoutInfo(arg1)  VhCreateFaninFanoutInfo((arg1),JAG_FILE,__LINE__)
#define vhGetSignalFaninList(arg1)  VhGetSignalFaninList((arg1),JAG_FILE,__LINE__)
#define vhGetSignalFanoutList(arg1)  VhGetSignalFanoutList((arg1),JAG_FILE,__LINE__)
#define  vhGetAllLibraries() VhGetAllLibraries(JAG_FILE,__LINE__)
#define  vhCloseUnit() VhCloseUnit(JAG_FILE,__LINE__)
#define  vhCloseFeature() VhCloseFeature(JAG_FILE,__LINE__)
#define  vhGetDisplayErrorOption() VhGetDisplayErrorOption(JAG_FILE,__LINE__)
#define  vhGetLastOccuredErrMessage() VhGetLastOccuredErrMessage(JAG_FILE,__LINE__)
#define  vhGetLastOccuredErrCode() VhGetLastOccuredErrCode(JAG_FILE,__LINE__)
#define  vhCreateNewMessage(arg1)  VhCreateNewMessage((arg1),JAG_FILE,__LINE__)
#define  vhModifyMessage(arg1,arg2)  VhModifyMessage((arg1),(vhString)(arg2),JAG_FILE,__LINE__)
#define vhModifyMsgSeverity(arg1, arg2)  VhModifyMsgSeverity((arg1),(arg2),JAG_FILE,__LINE__)
#define vhMsgNoGetMnemonic(arg1) VhMsgNoGetMnemonic((arg1),JAG_FILE,__LINE__)
#define vhModifyMnemonicMsgSeverity(arg1,arg2) VhModifyMnemonicMsgSeverity((arg1),(arg2),JAG_FILE,__LINE__)

#define vhGetAbsoluteBaseType(arg1)  VhGetAbsoluteBaseType((arg1),JAG_FILE,__LINE__)
#define vhGetSubTypeBaseType(arg1)  VhGetSubTypeBaseType((arg1),JAG_FILE,__LINE__)
#define vhDeleteNode(arg1)  VhDeleteNode((arg1),JAG_FILE,__LINE__)
#define vhMakeCreatedNodeDeleteable(arg1)  VhMakeCreatedNodeDeleteable(arg1)

#define vhSignalGetFanoutListAtIndex(arg1, arg2)  VhSignalGetFanoutListAtIndex((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSignalGetFaninListAtIndex(arg1, arg2)  VhSignalGetFaninListAtIndex((arg1),(arg2),JAG_FILE,__LINE__)
#define    vhNetBitGetFaninList(arg1) VhNetBitGetFaninList((arg1),JAG_FILE,__LINE__)
#define    vhNetBitGetFanoutList(arg1) VhNetBitGetFanoutList((arg1),JAG_FILE,__LINE__)
#define    vhNetBitGetNet(arg1) VhNetBitGetNet((arg1),JAG_FILE,__LINE__)
#define    vhNetBitGetName(arg1) VhNetBitGetName((arg1),JAG_FILE,__LINE__)
#define    vhNetBitGetIndex(arg1) VhNetBitGetIndex((arg1),JAG_FILE,__LINE__)
#define    vhTermBitGetNetBit(arg1) VhTermBitGetNetBit((arg1),JAG_FILE,__LINE__)
#define    vhTermBitGetPortBit(arg1) VhTermBitGetPortBit((arg1),JAG_FILE,__LINE__)
#define    vhPortBitGetPort(arg1) VhPortBitGetPort((arg1),JAG_FILE,__LINE__)
#define    vhPortBitGetNetBit(arg1) VhPortBitGetNetBit((arg1),JAG_FILE,__LINE__)
#define    vhPortBitGetName(arg1) VhPortBitGetName((arg1),JAG_FILE,__LINE__)
#define    vhTermBitGetTerminal(arg1) VhTermBitGetTerminal((arg1),JAG_FILE,__LINE__)

#define vhSignalGetNetBit(arg1, arg2)  VhSignalGetNetBit((arg1),(arg2),JAG_FILE,__LINE__)
#define vhPortGetPortBit(arg1, arg2)  VhPortGetPortBit((arg1),(arg2),JAG_FILE,__LINE__)
#define vhTerminalGetTermBit(arg1, arg2)  VhTerminalGetTermBit((arg1),(arg2),JAG_FILE,__LINE__)

#define vhInstanceGetTerminal(arg1, arg2)  VhInstanceGetTerminal((arg1),(arg2),JAG_FILE,__LINE__)
#define vhInstanceGetCorrespondingTerminal(arg1, arg2)  VhInstanceGetCorrespondingTerminal((arg1),(arg2),JAG_FILE,__LINE__)
#define vhSearchRegularExprInScope(arg1, arg2, arg3)  VhSearchRegularExprInScope((arg1),(arg2),(arg3),JAG_FILE,__LINE__)

#define vhPreserveActualNameInDump(arg1) VhPreserveActualNameInDump((arg1),JAG_FILE,__LINE__)
#define vhSetCheckSumDepCheck(arg1) VhSetCheckSumDepCheck((arg1),JAG_FILE,__LINE__)
#define vhSetCheckSumDepCheckWithInfo(arg1) VhSetCheckSumDepCheckWithInfo((arg1),JAG_FILE,__LINE__)
#define vhEnableCheckForOverwrite(arg1) VhEnableCheckForOverwrite((arg1),JAG_FILE,__LINE__)
#define vhEnableErrorForOverwrite(arg1) VhEnableErrorForOverwrite((arg1),JAG_FILE,__LINE__)
#define vhEnableUnLinkBeforeOverwrite(arg1) VhEnableUnLinkBeforeOverwrite((arg1),JAG_FILE,__LINE__)
#define vhKeepFirstEntArch(arg1) VhKeepFirstEntArch((arg1),JAG_FILE,__LINE__)
#define vhRemoveEntityAndArchFromDump(arg1, arg2) VhRemoveEntityAndArchFromDump((arg1),(arg2),JAG_FILE,__LINE__)
#define vhRemovePackDeclAndPackBodyFromDump(arg1, arg2) VhRemovePackDeclAndPackBodyFromDump((arg1),(arg2),JAG_FILE,__LINE__)
#define vhRemoveConfigFromDump(arg1, arg2) VhRemoveConfigFromDump((arg1),(arg2),JAG_FILE,__LINE__)
#define vhRemoveVunitFromDump(arg1, arg2) VhRemoveVunitFromDump((arg1),(arg2),JAG_FILE,__LINE__)
#define vhRegisterMvvRemovefromDump(arg1) VhRegisterMvvRemovefromDump((arg1),JAG_FILE,__LINE__)
#define vhIsUnitTop(arg1, arg2, arg3) VhIsUnitTop((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhCreateUserPragma(arg1, arg2, arg3, arg4) VhCreateUserPragma((arg1),(arg2),(arg3),(arg4),JAG_FILE,__LINE__)
#define vhInstanceHasConfigurationBindingAspect(arg1) VhInstanceHasConfigurationBindingAspect((arg1),JAG_FILE,__LINE__)
#define vhInstanceHasMultipleBindings(arg1) VhInstanceHasMultipleBindings((arg1),JAG_FILE,__LINE__)
#define vhIncludeWorkStd(arg1) VhIncludeWorkStd((arg1),JAG_FILE,__LINE__)
#define vhShowPortsWithinSynthRegion(arg1) VhShowPortsWithinSynthRegion((arg1),JAG_FILE,__LINE__)
#define vhIsSignalVector(arg1) VhIsSignalVector((arg1),JAG_FILE,__LINE__)
#define vhIsExprVector(arg1) VhIsExprVector((arg1),JAG_FILE,__LINE__)

#define vhTerminalGetPort(arg1) VhTerminalGetPort((arg1),JAG_FILE,__LINE__)
#define vhTerminalGetInstance(arg1) VhTerminalGetInstance((arg1),JAG_FILE,__LINE__)
#define vhTerminalGetTermBitList(arg1) VhTerminalGetTermBitList((arg1),JAG_FILE,__LINE__)
#define vhKeepNamedAssociationInfo() VhKeepNamedAssociationInfo(JAG_FILE,__LINE__)
#define vhPreserveSubProgAssocListInfo(arg1) VhPreserveSubProgAssocListInfo((arg1),JAG_FILE,__LINE__)
#define vhGetSubProgAssocListWithNamedInfo(arg1) VhGetSubProgAssocListWithNamedInfo((arg1),JAG_FILE,__LINE__)
#define vhGetInstanceGenericListWithNamedInfo(arg1) VhGetInstanceGenericListWithNamedInfo((arg1),JAG_FILE,__LINE__)
#define vhGetInstancePortListWithNamedInfo(arg1) VhGetInstancePortListWithNamedInfo((arg1),JAG_FILE,__LINE__)
#define vhIsNamedAssocNode(arg1) VhIsNamedAssocNode((arg1),JAG_FILE,__LINE__)
#define vhDontOmitFileOnNameClash(arg1) VhDontOmitFileOnNameClash((arg1),JAG_FILE,__LINE__)
#define vhSearchAllLibsForUnresolvedRefs(arg1) VhSearchAllLibsForUnresolvedRefs((arg1),JAG_FILE,__LINE__)
#define vhCreateDistinctBlackBox(arg1) VhCreateDistinctBlackBox((arg1),JAG_FILE, __LINE__)
#define vhGetLibListForUnresolvedEntities(arg1) VhGetLibListForUnresolvedEntities((arg1),JAG_FILE,__LINE__)
#define vhExplicitMsgForMultipleDecl(arg1) VhExplicitMsgForMultipleDecl((arg1),JAG_FILE,__LINE__)
#define vhMsgForMultipleAttributeDecl(arg1) VhMsgForMultipleAttributeDecl((arg1),JAG_FILE,__LINE__)
#define vhDecompileElaboratedLocStatRange(arg1) VhDecompileElaboratedLocStatRange((arg1),JAG_FILE,__LINE__)
#define vhIgnoreChoiceOutOfRange(arg1) VhIgnoreChoiceOutOfRange((arg1),JAG_FILE,__LINE__)
#define vhIgnoreMatchingCaseChoiceSemanticCheck(arg1) VhIgnoreMatchingCaseChoiceSemanticCheck((arg1),JAG_FILE,__LINE__)
#define vhIgnoreMatchingCaseSemanticCheckInsideGenerate(arg1) VhIgnoreMatchingCaseSemanticCheckInsideGenerate((arg1),JAG_FILE,__LINE__)
#define vhIgnoreMatchingCaseChoiceSemanticCheckInElab(arg1) VhIgnoreMatchingCaseChoiceSemanticCheckInElab((arg1),JAG_FILE,__LINE__)
#define vhIgnoreMatchingSelectedAsgnSemanticCheck(arg1) VhIgnoreMatchingSelectedAsgnSemanticCheck((arg1),JAG_FILE,__LINE__)
#define vhIgnoreMatchingSelectedAsgnSemanticCheckInsideGenerate(arg1) VhIgnoreMatchingSelectedAsgnSemanticCheckInsideGenerate((arg1),JAG_FILE,__LINE__)
#define vhIgnoreMatchingSelectedAsgnSemanticCheckInElab(arg1) VhIgnoreMatchingSelectedAsgnSemanticCheckInElab((arg1),JAG_FILE,__LINE__)
#define vhEnableEscapeSeqChar(arg1) VhEnableEscapeSeqChar((arg1),JAG_FILE,__LINE__)
#define vhCheckForSameSubTypeIndAndDeclName(arg1) VhCheckForSameSubTypeIndAndDeclName((arg1),JAG_FILE,__LINE__)
#define vhAllowPslEndpointIn2005Mode(arg1) VhAllowPslEndpointIn2005Mode((arg1),JAG_FILE,__LINE__)
#define vhDisableObjectReuse(arg1) VhDisableObjectReuse((arg1),JAG_FILE,__LINE__)
#define vhAllowRecordTypeNamePrefixInSelectedName(arg1) VhAllowRecordTypeNamePrefixInSelectedName((arg1),JAG_FILE,__LINE__)
#define vhIsAnalyzingProtectedRegion() VhIsAnalyzingProtectedRegion(JAG_FILE,__LINE__)
#define vhIsMatchingCaseStmt(arg1) VhIsMatchingCaseStmt((arg1),JAG_FILE,__LINE__)
#define vhSetMatchingCaseStmt(arg1, arg2) VhSetMatchingCaseStmt((arg1),(arg2),JAG_FILE,__LINE__)
#define vhIsLinkedToForeignAttr(arg1) VhIsLinkedToForeignAttr((arg1),JAG_FILE,__LINE__)
#define vhIsLinkedToStdForeignAttr(arg1) VhIsLinkedToStdForeignAttr((arg1),JAG_FILE,__LINE__)
#define vhIncludeReportSeverityInSelectedPslDirectives(arg1) VhIncludeReportSeverityInSelectedPslDirectives((arg1),JAG_FILE,__LINE__)
#define vhCheckDeclParamAlsoInSubprogUsesGlobals(arg1) VhCheckDeclParamAlsoInSubprogUsesGlobals((arg1),JAG_FILE,__LINE__)
#define vhEnableSemicolonSeqChar(arg1) VhEnableSemicolonSeqChar((arg1),JAG_FILE,__LINE__)
#define vhDontAllowTwoPrimaryDUsWithSameNameInLibrary(arg1) VhDontAllowTwoPrimaryDUsWithSameNameInLibrary((arg1),JAG_FILE,__LINE__)
#define vhEnablePredefinedAttribSynthCheckStd2004(arg1) VhEnablePredefinedAttribSynthCheckStd2004((arg1),JAG_FILE,__LINE__)
#define vhAllowUnconnectedFormalPort(arg1) VhAllowUnconnectedFormalPort((arg1),JAG_FILE,__LINE__)
#define vhForcePortInitValueCopy(arg1) VhForcePortInitValueCopy((arg1),JAG_FILE,__LINE__)
#define vhSetFuncCallSize(arg1,arg2) VhSetFuncCallSize((arg1),(arg2),JAG_FILE,__LINE__)
#define vhAllowNonSignalElementInPortMapAspectIn87Mode(arg1) VhAllowNonSignalElementInPortMapAspectIn87Mode((arg1),JAG_FILE,__LINE__)
#define vhAllow87ModeSubProgOverloading(arg1) VhAllow87ModeSubProgOverloading((arg1),JAG_FILE,__LINE__)
#define vhGetBucketNumberForNode(arg1) VhGetBucketNumberForNode((arg1),JAG_FILE,__LINE__)
#define vhGetNodeFromBucketNumber(arg1,arg2,arg3,arg4,arg5) VhGetNodeFromBucketNumber((arg1),(arg2), (arg3), (arg4), (arg5),JAG_FILE,__LINE__)
#define vhSetMaxIdentifierLimit(arg1) VhSetMaxIdentifierLimit((arg1),JAG_FILE,__LINE__)
#define vhGetBitStringLength(arg1)  VhGetBitStringLength((arg1),JAG_FILE,__LINE__)
#define vhAllowDivideByZeroErrInAnalysis(arg1)  VhAllowDivideByZeroErrInAnalysis((arg1),JAG_FILE,__LINE__)
#define vhAllowGloballyStaticInFormal(arg1)  VhAllowGloballyStaticInFormal((arg1),JAG_FILE,__LINE__)
#define vhDisableShortCktCheckInEval(arg1)   VhDisableShortCktCheckInEval((arg1),JAG_FILE,__LINE__)
#define vhEnableEEAddressBasedExprEval(arg1)   VhEnableEEAddressBasedExprEval((arg1),JAG_FILE,__LINE__)
#define vhSetUseEntNameAsBBNameForEntWithoutArch(arg1)   VhSetUseEntNameAsBBNameForEntWithoutArch((arg1),JAG_FILE,__LINE__)
#define vhEvaluateFullyNonStaticExpr(arg1)   VhEvaluateFullyNonStaticExpr((arg1),JAG_FILE,__LINE__)
#define vhEnableFeedThruFuncFlowCrtl(arg1)   VhEnableFeedThruFuncFlowCrtl((arg1),JAG_FILE,__LINE__)
#define vhIgnoreSubprogVarDefaultInitialValue(arg1)   VhIgnoreSubprogVarDefaultInitialValue((arg1),JAG_FILE,__LINE__)
#define vhIsEEAddressBasedEval()   VhIsEEAddressBasedEval(JAG_FILE,__LINE__)
#define vhCreateStdLogicStringFromEEAddr(arg1)   VhCreateStdLogicStringFromEEAddr((arg1),JAG_FILE,__LINE__)
#define vhGetStdLogicCharFromEEAddr(arg1)   VhGetStdLogicCharFromEEAddr((arg1),JAG_FILE,__LINE__)
#define vhIgnore2008Keyword(arg1)   VhIgnore2008Keyword((arg1),JAG_FILE,__LINE__)
#define vhEvalFuncHavingREAD(arg1)   VhEvalFuncHavingREAD((arg1),JAG_FILE,__LINE__)
#define vhSetEvaluatingActualForInstConnections(arg1)   VhSetEvaluatingActualForInstConnections((arg1),JAG_FILE,__LINE__)
#define vhSetCurrentRecursiveEntity(arg1)   VhSetCurrentRecursiveEntity((arg1),JAG_FILE,__LINE__)
#define vhForceAggCreationForStr(arg1)   VhForceAggCreationForStr((arg1),JAG_FILE,__LINE__)
#define vhAllowOpenInInputPortFormalSubElement(arg1)  VhAllowOpenInInputPortFormalSubElement((arg1),JAG_FILE,__LINE__)
#define vhIsSuppressibleError(arg1) VhIsSuppressibleError((arg1),JAG_FILE,__LINE__)
#define vhIs2008ModeOn() VhIs2008ModeOn(JAG_FILE,__LINE__)
//#define vhDisableVhdl0208Mix(arg1) VhDisableVhdl0208Mix((arg1), JAG_FILE,__LINE__)
#define vhEnableVhdl0208Mix(arg1) VhEnableVhdl0208Mix((arg1), JAG_FILE,__LINE__)
#define vhIsVhdl0208MixEnabled() VhIsVhdl0208MixEnabled(JAG_FILE,__LINE__)
#define vhGet2008Type(arg1) VhGet2008Type((arg1))

#define    vhwGetCreateScope() VhwGetCreateScope(JAG_FILE,__LINE__)
#define    vhwSetCreateScope(arg1,arg2) VhwSetCreateScope((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateAggregate(arg1,arg2) VhwCreateAggregate((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhCreateProperAggregate(arg1,arg2) VhCreateProperAggregate((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateAlias(arg1,arg2,arg3,arg4) VhwCreateAlias((arg1), (arg2), (arg3), (arg4),JAG_FILE,__LINE__)
#define    vhwCreateGeneric(arg1) VhwCreateGeneric ((arg1),JAG_FILE,__LINE__)
#define    vhwCreateGenericMap(arg1) VhwCreateGenericMap ((arg1),JAG_FILE,__LINE__)
#define    vhwCreateLabel(arg1,arg2) VhwCreateLabel ((arg1), (arg2),JAG_FILE,__LINE__)
#define    vhwCreateLiteral(arg1,arg2) VhwCreateLiteral ((arg1), (arg2),JAG_FILE,__LINE__)
#define    vhwCreatePort(arg1) VhwCreatePort ((arg1),JAG_FILE,__LINE__)
#define    vhwCreatePortMap(arg1) VhwCreatePortMap ((arg1),JAG_FILE,__LINE__)
#define    vhwCreateSignature(arg1,arg2) VhwCreateSignature ((arg1), (arg2),JAG_FILE,__LINE__)
#define    vhwCreateString(arg1,arg2) VhwCreateString ((arg1), (arg2),JAG_FILE,__LINE__)
#define    vhwCreateUseObjectFrom(arg1,arg2) VhwCreateUseObjectFrom ((arg1), (arg2),JAG_FILE,__LINE__)
#define    vhwCreateWaveFormElement(arg1,arg2) VhwCreateWaveFormElement ((arg1), (arg2),JAG_FILE,__LINE__)
#define    vhwCreateWaveForm(arg1) VhwCreateWaveForm ((arg1),JAG_FILE,__LINE__)
#define    vhwCreateConfigSpec(arg1, arg2) VhwCreateConfigSpec((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateCompSpec(arg1, arg2) VhwCreateCompSpec((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateConstant(arg1, arg2, arg3, arg4, arg5, arg6) VhwCreateConstant((arg1), (arg2), (arg3), (arg4), (arg5), (arg6), JAG_FILE,__LINE__)
#define    vhwCreateSignal(arg1, arg2, arg3, arg4, arg5) VhwCreateSignal((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateExternalName(arg1, arg2, arg3, arg4, arg5) VhwCreateExternalName((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateVariable(arg1, arg2, arg3, arg4, arg5) VhwCreateVariable((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateFileDecl(arg1, arg2, arg3, arg4, arg5) VhwCreateFileDecl((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateComponent(arg1, arg2, arg3) VhwCreateComponent((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateAttrbDecl(arg1, arg2) VhwCreateAttrbDecl((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateAttributeSpec(arg1, arg2, arg3, arg4, arg5) VhwCreateAttributeSpec((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateForIndex(arg1, arg2) VhwCreateForIndex((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateSubProgDecl(arg1, arg2, arg3, arg4, arg5, arg6, arg7) VhwCreateSubProgDecl((arg1), (arg2), (arg3), (arg4), (arg5), (arg6), (arg7), JAG_FILE,__LINE__)
#define    vhwCreateSubProgBody(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) VhwCreateSubProgBody((arg1), (arg2), (arg3), (arg4),(arg5), (arg6), (arg7), (arg8), JAG_FILE,__LINE__)
#define    vhwCreateTypeDecl(arg1, arg2, arg3) VhwCreateTypeDecl((arg1), (arg2), JAG_FILE,__LINE__, (arg3))
#define    vhwCreateSubTypeDecl(arg1, arg2) VhwCreateSubTypeDecl((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreatePrimaryUnit(arg1, arg2) VhwCreatePrimaryUnit((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateSecondaryUnit(arg1, arg2, arg3) VhwCreateSecondaryUnit((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateEnumElement(arg1, arg2, arg3) VhwCreateEnumElement((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateElementDecl(arg1, arg2) VhwCreateElementDecl((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreatePhysicalType(arg1, arg2, arg3) VhwCreatePhysicalType((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateAccessType(arg1) VhwCreateAccessType((arg1), JAG_FILE,__LINE__)
#define    vhwCreateFileType(arg1) VhwCreateFileType((arg1), JAG_FILE,__LINE__)
#define    vhwCreateEnumType(arg1) VhwCreateEnumType((arg1), JAG_FILE,__LINE__)
#define    vhwCreateIntegerType(arg1) VhwCreateIntegerType((arg1), JAG_FILE,__LINE__)
#define    vhwCreateFloatType(arg1) VhwCreateFloatType((arg1), JAG_FILE,__LINE__)
#define    vhwCreateConstrainedArray(arg1, arg2, arg3) VhwCreateConstrainedArray((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateUnConstrainedArray(arg1, arg2, arg3) VhwCreateUnConstrainedArray((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateIndexSubTypeDef(arg1) VhwCreateIndexSubTypeDef((arg1), JAG_FILE, __LINE__)
#define    vhwCreateMultiDArraySubtype(arg1, arg2) VhwCreateMultiDArraySubtype((arg1), (vhString)(arg2), JAG_FILE, __LINE__)
#define    vhwCreateRecord(arg1) VhwCreateRecord((arg1), JAG_FILE,__LINE__)
#define    vhwCreateSubTypeInd(arg1, arg2, arg3, arg4) VhwCreateSubTypeInd((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateRange(arg1, arg2, arg3) VhwCreateRange((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateIndexConstraint(arg1) VhwCreateIndexConstraint((arg1), JAG_FILE,__LINE__)
#define    vhwCreateArrayConstraint(arg1,arg2) VhwCreateArrayConstraint((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateRecordConstraint(arg1) VhwCreateRecordConstraint((arg1), JAG_FILE,__LINE__)
#define    vhwCreateRecordElementConstraint(arg1,arg2) VhwCreateRecordElementConstraint((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateRangeConstraint(arg1) VhwCreateRangeConstraint((arg1), JAG_FILE,__LINE__)
#define    vhwCreateUnary(arg1, arg2, arg3) VhwCreateUnary((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateBinary(arg1, arg2, arg3, arg4) VhwCreateBinary((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateFuncCall(arg1, arg2, arg3) VhwCreateFuncCall((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateFuncNode(arg1, arg2) VhwCreateFuncNode((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateQualifiedExpr(arg1, arg2) VhwCreateQualifiedExpr((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateTypeConversion(arg1, arg2) VhwCreateTypeConversion((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateIndexName(arg1, arg2, arg3, arg4) VhwCreateIndexName((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateSliceName(arg1, arg2, arg3) VhwCreateSliceName((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateSliceNode(arg1, arg2) VhwCreateSliceNode((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreatePhysicalLiteral(arg1, arg2) VhwCreatePhysicalLiteral((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateBitString(arg1, arg2, arg3) VhwCreateBitString((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateOpString(arg1, arg2) VhwCreateOpString((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateAttributeName(arg1, arg2, arg3, arg4, arg5, arg6) VhwCreateAttributeName((arg1), (arg2), (arg3), (arg4), (arg5), (arg6), JAG_FILE,__LINE__)
#define    vhwCreateVhObject(arg1) VhwCreateVhObject((arg1), JAG_FILE,__LINE__)
#define    vhwCreateSelectedName(arg1, arg2, arg3) VhwCreateSelectedName((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateSimpleName(arg1, arg2) VhwCreateSimpleName((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateElementAssociation(arg1, arg2) VhwCreateElementAssociation((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateOthers VhwCreateOthers(JAG_FILE,__LINE__)
#define    vhwCreateAll() VhwCreateAll(JAG_FILE,__LINE__)
#define    vhwCreateOpen() VhwCreateOpen(JAG_FILE,__LINE__)
#define    vhwCreateIdEnumLit(arg1, arg2) VhwCreateIdEnumLit((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateCharLit(arg1, arg2) VhwCreateCharLit((arg1), (arg2),  JAG_FILE,__LINE__)
#define    vhwCreateDecLit(arg1, arg2) VhwCreateDecLit((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateBaseLit(arg1, arg2, arg3) VhwCreateBaseLit((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateNullLit() VhwCreateNullLit(JAG_FILE,__LINE__)
#define    vhIsSignalVector(arg1) VhIsSignalVector((arg1),JAG_FILE,__LINE__)
#define    vhIsExprVector(arg1) VhIsExprVector((arg1),JAG_FILE,__LINE__)
#define    vhwSetExprBound(arg1, arg2)   VhwSetExprBound((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwSetAliasedExpression(arg1, arg2)   VhwSetAliasedExpression((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateAssociation(arg1, arg2) VhwCreateAssociation((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateBindingIndication(arg1, arg2, arg3) VhwCreateBindingIndication((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateEntityAspect(arg1, arg2, arg3, arg4) VhwCreateEntityAspect((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateDesignUnit(arg1, arg2, arg3, arg4) VhwCreateDesignUnit((vhString)(arg1), (vhString)(arg2), (vhString)(arg3), (arg4), JAG_FILE,__LINE__)
#define    vhAddLogicalLibraryToMap(arg1, arg2) VhAddLogicalLibraryToMap((vhString)(arg1), (vhString)(arg2))
#define    vhSetJaguarWorkLibrary(arg1) VhSetJaguarWorkLibrary((vhString)(arg1))
#define    vhwCreateComponentInstance(arg1, arg2, arg3, arg4, arg5) VhwCreateComponentInstance((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateEntityInstance(arg1, arg2, arg3, arg4, arg5, arg6) VhwCreateEntityInstance((arg1), (arg2), (arg3), (arg4), (arg5), (arg6), JAG_FILE,__LINE__)
#define    vhwCreateConfigurationInstance(arg1, arg2, arg3, arg4, arg5) VhwCreateConfigurationInstance((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateBlock(arg1, arg2) VhwCreateBlock((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateBlockConfig(arg1) VhwCreateBlockConfig((arg1), JAG_FILE,__LINE__)
#define    vhwCreateProtectedBody(arg1) VhwCreateProtectedBody((arg1), JAG_FILE,__LINE__)
#define    vhwCreateProtectedDecl(arg1) VhwCreateProtectedDecl((arg1), JAG_FILE,__LINE__)
#define    vhwCreateCompConfig(arg1,arg2) VhwCreateCompConfig((arg1),(arg2), JAG_FILE,__LINE__)
#define  vhSetConfigItemList(arg1,arg2) VhSetConfigItemList((arg1),(arg2), JAG_FILE,__LINE__)
#define    vhwCreateProcess(arg1, arg2) VhwCreateProcess((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateConcProcCall(arg1, arg2) VhwCreateConcProcCall((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateConcSignalAssignment(arg1, arg2, arg3, arg4) VhwCreateConcSignalAssignment((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateCondSignalAssignment(arg1, arg2, arg3, arg4) VhwCreateCondSignalAssignment((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateSelectedSignalAssignment(arg1, arg2, arg3, arg4, arg5) VhwCreateSelectedSignalAssignment((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateIfGenerate(arg1, arg2) VhwCreateIfGenerate((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateForGenerate(arg1, arg2) VhwCreateForGenerate((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateElsIfGenerate(arg1, arg2, arg3) VhwCreateElsIfGenerate((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateElseGenerate(arg1,arg2) VhwCreateElseGenerate((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateCaseGenerate(arg1, arg2, arg3) VhwCreateCaseGenerate((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateCaseGenAlternative(arg1, arg2) VhwCreateCaseGenAlternative((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateConcAssertion(arg1, arg2) VhwCreateConcAssertion((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateWait(arg1, arg2, arg3, arg4) VhwCreateWait((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateVariableAssignment(arg1, arg2, arg3) VhwCreateVariableAssignment((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateSignalAssignment(arg1, arg2, arg3, arg4, arg5, arg6, arg7) VhwCreateSignalAssignment((arg1), (arg2), (arg3), (arg4), (arg5), (arg6), (arg7), JAG_FILE,__LINE__)
#define    vhwCreateSignalForceAssignment(arg1, arg2, arg3, arg4, arg5) VhwCreateSignalForceAssignment((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateSignalReleaseAssignment(arg1, arg2, arg3, arg4) VhwCreateSignalReleaseAssignment((arg1), (arg2), (arg3), (arg4),  JAG_FILE,__LINE__)
#define    vhwCreateProcedureCall(arg1, arg2, arg3) VhwCreateProcedureCall((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateIfStmt(arg1, arg2, arg3, arg4, arg5) VhwCreateIfStmt((arg1), (arg2), (arg3), (arg4), (arg5), JAG_FILE,__LINE__)
#define    vhwCreateElsIfStmt(arg1, arg2) VhwCreateElsIfStmt((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateElseStmt(arg1, arg2) VhwCreateElseStmt((arg1), JAG_FILE,__LINE__)
#define    vhwCreateCaseStmt(arg1, arg2, arg3) VhwCreateCaseStmt((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateCaseAlternative(arg1, arg2) VhwCreateCaseAlternative((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateLoopStmt(arg1, arg2) VhwCreateLoopStmt((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateWhileStmt(arg1, arg2, arg3) VhwCreateWhileStmt((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateForStmt(arg1, arg2, arg3) VhwCreateForStmt((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateAssertion(arg1, arg2, arg3, arg4) VhwCreateAssertion((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateReport(arg1, arg2, arg3) VhwCreateReport((arg1), (arg2), (arg3), JAG_FILE,__LINE__)
#define    vhwCreateNext(arg1, arg2, arg3, arg4) VhwCreateNext((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateExit(arg1, arg2, arg3, arg4) VhwCreateExit((arg1), (arg2), (arg3), (arg4), JAG_FILE,__LINE__)
#define    vhwCreateReturn(arg1, arg2) VhwCreateReturn((arg1), (arg2), JAG_FILE,__LINE__)
#define    vhwCreateNull(arg1) VhwCreateNull((arg1), JAG_FILE,__LINE__)
#define    vhwCreateLibraryClause(arg1) VhwCreateLibraryClause((arg1), JAG_FILE,__LINE__)
#define    vhArchElaborateGenerateStatement(pArch) VhArchElaborateGenerateStatement(pArch, JAG_FILE, __LINE__)
#define    vhArchUnElaborateGenerateStatement(pArch) VhArchUnElaborateGenerateStatement(pArch, JAG_FILE, __LINE__)
#define    vhRegisterDumpFileHandler(function) VhRegisterDumpFileHandler(function, JAG_FILE, __LINE__)
#define    vhCopyFileNameAndLineNo(flag) VhCopyFileNameAndLineNo(flag, JAG_FILE, __LINE__)
#define    vhPslVerifUnitGetUnitType(arg1) VhPslVerifUnitGetUnitType((arg1), JAG_FILE, __LINE__)
#define    vhIsDefaultPslVerifUnit(arg1) VhIsDefaultPslVerifUnit((arg1), JAG_FILE, __LINE__)
#define    vhPslVerifUnitGetUnitName(arg1) VhPslVerifUnitGetUnitName((arg1), JAG_FILE, __LINE__)
#define    vhPslVerifUnitGetUnitUnderVerification(arg1) VhPslVerifUnitGetUnitUnderVerification((arg1), JAG_FILE, __LINE__)
#define    vhPslVerifUnitGetInheritedUnitList(arg1) VhPslVerifUnitGetInheritedUnitList((arg1), JAG_FILE, __LINE__)
#define    vhPslVerifUnitGetItemList(arg1) VhPslVerifUnitGetItemList((arg1), JAG_FILE, __LINE__)
#define    vhPslGetClockExpr(arg1) VhPslGetClockExpr((arg1), JAG_FILE, __LINE__)
#define    vhPslNamedDeclGetFormalList(arg1) VhPslNamedDeclGetFormalList((arg1), JAG_FILE, __LINE__)
#define    vhPslParamGetParamKind(arg1) VhPslParamGetParamKind((arg1), JAG_FILE, __LINE__)
#define    vhPslParamIsConst(arg1) VhPslParamIsConst((arg1), JAG_FILE, __LINE__)
#define    vhPslParamGetSubTypeInd(arg1) VhPslParamGetSubTypeInd((arg1), JAG_FILE, __LINE__)
#define    vhPslDeclGetName(arg1) VhPslDeclGetName((arg1), JAG_FILE, __LINE__)
#define    vhPslStmtGetDefaultClock(arg1) VhPslStmtGetDefaultClock((arg1), JAG_FILE, __LINE__)
#define    vhPslGetProperty(arg1) VhPslGetProperty((arg1), JAG_FILE, __LINE__)
#define    vhPslGetSequence(arg1) VhPslGetSequence((arg1), JAG_FILE, __LINE__)
#define    vhPslFairnessGetExpr(arg1) VhPslFairnessGetExpr((arg1), JAG_FILE, __LINE__)
#define    vhPslFairnessGetStrongExpr(arg1) VhPslFairnessGetStrongExpr((arg1), JAG_FILE, __LINE__)
#define    vhPslForAllGetForAllIndex(arg1)  VhPslForAllGetForAllIndex((arg1), JAG_FILE, __LINE__)
#define    vhPslForAllIndexGetIndexRange(arg1) VhPslForAllIndexGetIndexRange((arg1), JAG_FILE, __LINE__)
#define    vhPslForAllIndexGetValueSet(arg1) VhPslForAllIndexGetValueSet((arg1), JAG_FILE, __LINE__)
#define    vhPslPropertyOperatorGetOpType(arg1) VhPslPropertyOperatorGetOpType((arg1), JAG_FILE, __LINE__)
#define    vhPslPropertyOperatorGetBound(arg1) VhPslPropertyOperatorGetBound((arg1), JAG_FILE, __LINE__)
#define    vhPslExprGetOperand(arg1) VhPslExprGetOperand((arg1), JAG_FILE, __LINE__)
#define    vhPslExprGetLeftOperand(arg1) VhPslExprGetLeftOperand((arg1), JAG_FILE, __LINE__)
#define    vhPslExprGetRightOperand(arg1) VhPslExprGetRightOperand((arg1), JAG_FILE, __LINE__)
#define    vhPslExprGetMiddleOperand(arg1) VhPslExprGetMiddleOperand((arg1), JAG_FILE, __LINE__)
#define    vhPslInstanceGetName(arg1) VhPslInstanceGetName((arg1), JAG_FILE, __LINE__)
#define    vhPslInstanceGetMaster(arg1) VhPslInstanceGetMaster((arg1), JAG_FILE, __LINE__)
#define    vhPslInstanceGetActualParamList(arg1) VhPslInstanceGetActualParamList((arg1), JAG_FILE, __LINE__)
#define    vhPslSereGetRepeatType(arg1) VhPslSereGetRepeatType((arg1), JAG_FILE, __LINE__)
#define    vhPslSereGetCompositType(arg1) VhPslSereGetCompositType((arg1), JAG_FILE, __LINE__)
#define    vhPslSereConcatSEREList(arg1) VhPslSereConcatSEREList((arg1), JAG_FILE, __LINE__)
#define    vhPslFuncCallGetAssociationList(arg1) VhPslFuncCallGetAssociationList((arg1), JAG_FILE, __LINE__)
#define    vhPslFuncCallGetFuncType(arg1) VhPslFuncCallGetFuncType((arg1), JAG_FILE, __LINE__)
#define    vhDuGetPslDeclList(arg1) VhDuGetPslDeclList((arg1), JAG_FILE, __LINE__)
#define    vhDuGetPslStmtList(arg1) VhDuGetPslStmtList((arg1), JAG_FILE, __LINE__)
#define    vhGetPslDeclList(arg1) VhGetPslDeclList((arg1), JAG_FILE, __LINE__)
#define    vhGetPslStmtList(arg1) VhGetPslStmtList((arg1), JAG_FILE, __LINE__)
#define    vhPslLoadVerificationUnit(arg1, arg2) VhPslLoadVerificationUnit((arg1), (arg2), JAG_FILE, __LINE__)
#define    vhPslUnrollForAll(arg1) VhPslUnrollForAll((arg1), JAG_FILE, __LINE__)
#define    vhPslUpdateForAll(arg1) VhPslUpdateForAll((arg1), JAG_FILE, __LINE__)
#define    vhPslForAllGetCurrVal(arg1) VhPslForAllGetCurrVal((arg1), JAG_FILE, __LINE__)
#define    vhPslResetForAll(arg1) VhPslResetForAll((arg1), JAG_FILE, __LINE__)
#define vhGetPslPercentIfCondition(arg1)  VhGetPslPercentIfCondition((arg1),JAG_FILE,__LINE__)
#define vhGetPslPercentIfThenText(arg1)  VhGetPslPercentIfThenText((arg1),JAG_FILE,__LINE__)
#define vhGetPslPercentIfElseText(arg1)  VhGetPslPercentIfElseText((arg1),JAG_FILE,__LINE__)
#define vhGetPslPercentForReplicatorName(arg1)  VhGetPslPercentForReplicatorName((arg1),JAG_FILE,__LINE__)
#define vhGetPslPercentForText(arg1)  VhGetPslPercentForText((arg1),JAG_FILE,__LINE__)
#define vhIsPslPercentForReplicatorARange(arg1)  VhIsPslPercentForReplicatorARange((arg1),JAG_FILE,__LINE__)
#define vhGetPslPercentForReplicatorValues(arg1)  VhGetPslPercentForReplicatorValues((arg1),JAG_FILE,__LINE__)
#define    vhDuGetPslMacroList(arg1) VhDuGetPslMacroList((arg1), JAG_FILE, __LINE__)
#define    vhGetPslMacroList(arg1) VhGetPslMacroList((arg1), JAG_FILE, __LINE__)
#define    vhUseWorkForExt(arg1) VhUseWorkForExt((arg1), JAG_FILE, __LINE__)
#define vhSetEntityNameForCopiedArch(arg1,arg2) VhSetEntityNameForCopiedArch((arg1), (arg2), JAG_FILE, __LINE__)
#define vhSetMraNameForCopiedEntity(arg1,arg2) VhSetMraNameForCopiedEntity((arg1), (arg2), JAG_FILE, __LINE__)
#define vhSetEntityForArchCopy(arg1) VhSetEntityForArchCopy((arg1),JAG_FILE, __LINE__)
#define vhAliasSubprogram(arg1) VhAliasSubprogram((arg1),JAG_FILE, __LINE__)
#define vhIgnoreElabSizeMismatch(arg1) VhIgnoreElabSizeMismatch((arg1),JAG_FILE, __LINE__)
#define vhAllowSizeMismatchInElabAnalysis(arg1) VhAllowSizeMismatchInElabAnalysis((arg1),JAG_FILE, __LINE__)
#define vhPickMostRecentConfig(arg1) VhPickMostRecentConfig((arg1),JAG_FILE, __LINE__)
#define vhGetPslVUnitBoundUnitLibraryName(arg1) VhGetPslVUnitBoundUnitLibraryName((arg1),JAG_FILE, __LINE__)
#define vhGetPslVUnitBoundUnitDesignUnit(arg1) VhGetPslVUnitBoundUnitDesignUnit((arg1),JAG_FILE, __LINE__)
#define vhGetPslVUnitBoundUnitInstanceList(arg1) VhGetPslVUnitBoundUnitInstanceList((arg1),JAG_FILE, __LINE__)
#define vhPslBindVerificationUnitDuringRestore() VhPslBindVerificationUnitDuringRestore()
#define vhPslUnBindVerificationUnitDuringRestore() VhPslUnBindVerificationUnitDuringRestore()
#define vhPslDUGetBindedVerificationUnitNameList(arg1) VhPslDUGetBindedVerificationUnitNameList((arg1),JAG_FILE, __LINE__)
#define vhPslInstanceGetBindedVerificationUnitNameList(arg1,arg2,arg3) VhPslInstanceGetBindedVerificationUnitNameList((arg1), (arg2), (arg3), JAG_FILE, __LINE__)
#define vhPslElabBindedVerificationUnit(arg1,arg2) VhPslElabBindedVerificationUnit((arg1), (arg2), JAG_FILE, __LINE__)
#define vhPslInstanceGetBindedVUnitNameListUsingElabEntArch(arg1,arg2,arg3,arg4) VhPslInstanceGetBindedVUnitNameListUsingElabEntArch((arg1), (arg2), (arg3), (arg4), JAG_FILE, __LINE__)
#define vhPslElabBindedVUnitUsingElabEntArch(arg1,arg2,arg3) VhPslElabBindedVUnitUsingElabEntArch((arg1), (arg2), (arg3), JAG_FILE, __LINE__)
#define vhGetCurrentDesignUnitNode() VhGetCurrentDesignUnitNode(JAG_FILE, __LINE__)
#define vhOverrideGenericValue(arg1, arg2, arg3) VhOverrideGenericValue((arg1), (arg2), (arg3), JAG_FILE, __LINE__)
#define vhResetOverriddenGeneric(arg1, arg2) VhResetOverriddenGeneric((arg1), (arg2), JAG_FILE, __LINE__)
#define vhEnableGenericOverride(arg1) VhEnableGenericOverride((arg1),JAG_FILE, __LINE__)
#define vhAllowConstantActualForAllPortTypes(arg1) VhAllowConstantActualForAllPortTypes((arg1),JAG_FILE, __LINE__)
#define vhCheckForStaticGenVal(arg1) VhCheckForStaticGenVal((arg1),JAG_FILE, __LINE__)
#define    vhPslPslExprOperatorGetOpType(arg1) VhPslPslExprOperatorGetOpType((arg1), JAG_FILE, __LINE__)
#define    vhPslGetReportString(arg1) VhPslGetReportString((arg1), JAG_FILE, __LINE__)
#define    vhPslGetSeverityString(arg1) VhPslGetSeverityString((arg1), JAG_FILE, __LINE__)
#define vhSetConcordeCompatibility(arg1) VhSetConcordeCompatibility((arg1),JAG_FILE, __LINE__)
#define vhGetReadSignalsNotInSensList(arg1) VhGetReadSignalsNotInSensList((arg1), JAG_FILE, __LINE__)
#define    vhPslSequenceGetIsStrong(arg1) VhPslSequenceGetIsStrong((arg1), JAG_FILE, __LINE__)
#define vhBlockIsGenerateBlock(arg1) VhBlockIsGenerateBlock((arg1), JAG_FILE, __LINE__)
#define vhBlockGetOriginalName(arg1) VhBlockGetOriginalName((arg1), JAG_FILE, __LINE__)
#define vhBlockGetGenerateIndex(arg1) VhBlockGetGenerateIndex((arg1), JAG_FILE, __LINE__)
#define vhEnableSingleLineErrorMessage(arg1) VhEnableSingleLineErrorMessage((arg1), JAG_FILE, __LINE__)
#define vhEnableUnitClosureDuringTopDetection(arg1) VhEnableUnitClosureDuringTopDetection((arg1), JAG_FILE, __LINE__)
#define vhEnableEntHeaderInOpenDeclRegn(arg1) VhEnableEntHeaderInOpenDeclRegn((arg1), JAG_FILE, __LINE__)
#define vhAllowBackWardCompatibility(arg1, arg2) VhAllowBackWardCompatibility((arg1), (arg2), JAG_FILE, __LINE__)
#define vhEnableSymTabAddition(arg1) VhEnableSymTabAddition((arg1), JAG_FILE, __LINE__)
#define vhEnableSynthErrWarnUnderExtMsgHandler(arg1) VhEnableSynthErrWarnUnderExtMsgHandler((arg1), JAG_FILE, __LINE__)
#define vhCreateDsEntBlockConfigList(arg1) VhCreateDsEntBlockConfigList((arg1), JAG_FILE, __LINE__)
#define vhEnableRtlSynthesisOffOnAsSynthesisRegion(arg1) VhEnableRtlSynthesisOffOnAsSynthesisRegion((arg1), JAG_FILE, __LINE__)
#define vhSetEntPatternToIgnoreTransOffOn(arg1) VhSetEntPatternToIgnoreTransOffOn((arg1), JAG_FILE, __LINE__)
#define vhSetActualImpurity(arg1) VhSetActualImpurity((arg1), JAG_FILE, __LINE__)
#define vhRegisterPreParseFileHandler(arg1, arg2, arg3) VhRegisterPreParseFileHandler((arg1), (arg2), (arg3), JAG_FILE, __LINE__)
#define vhUseCreateFileNameForNewNodes(arg1) VhUseCreateFileNameForNewNodes((arg1), JAG_FILE, __LINE__)
#define vhRegisterProtectDecryptionCallBack(arg1) VhRegisterProtectDecryptionCallBack((arg1), JAG_FILE, __LINE__)
#define vhRegisterProtectNextDecryptionCallBack(arg1) VhRegisterProtectNextDecryptionCallBack((arg1), JAG_FILE, __LINE__)
#define vhRegisterProtectedInputFilter(arg1) VhRegisterProtectedInputFilter((arg1), JAG_FILE, __LINE__)
#define vhRegisterDeallocatorForProtectedInputText(arg1) VhRegisterDeallocatorForProtectedInputText((arg1), JAG_FILE, __LINE__)
#define vhRegisterEvaluateRtlc(arg1) VhRegisterEvaluateRtlc((arg1), JAG_FILE, __LINE__)
#define vhRegisterEvaluateRtlcToCheckForNonStaticExpr(arg1) VhRegisterEvaluateRtlcToCheckForNonStaticExpr((arg1), JAG_FILE, __LINE__)
#define vhRegisterCallbackForEndDU(arg1) VhRegisterCallbackForEndDU((arg1), JAG_FILE, __LINE__)
#define vhRegisterCommentForProtectedInputText(arg1) VhRegisterCommentForProtectedInputText((arg1), JAG_FILE, __LINE__)
#define vhRegisterCallbackForEndPackage(arg1) VhRegisterCallbackForEndPackage((arg1), JAG_FILE, __LINE__)
#define vhDefineProtectedRegionMarkers(arg1, arg2) VhDefineProtectedRegionMarkers((arg1), (arg2), JAG_FILE, __LINE__)
#define vhIsEncrypted(arg1) VhIsEncrypted((arg1), JAG_FILE, __LINE__)
#define vhSortDsEntities(arg1) VhSortDsEntities((arg1), JAG_FILE, __LINE__)
#define vhEnablePSLElaborationInDsElab(arg1) VhEnablePSLElaborationInDsElab((arg1), JAG_FILE, __LINE__)
#define vhDontStoreDesignUnitComments(arg1) VhDontStoreDesignUnitComments((arg1), JAG_FILE, __LINE__)
#define vhIgnorePslConstructs(arg1) VhIgnorePslConstructs((arg1), JAG_FILE, __LINE__)
#define vhAllowMultipleNameSpace(arg1) VhAllowMultipleNameSpace((arg1), JAG_FILE, __LINE__)
#define vhCheckLogicalAndPhyLibToOverwriteDU(arg1) VhCheckLogicalAndPhyLibToOverwriteDU((arg1), JAG_FILE, __LINE__)
#define vhPslGetNameTypeOfBindUnitExpr(arg1)  VhPslGetNameTypeOfBindUnitExpr((arg1),JAG_FILE,__LINE__)
#define vhPslGetPrimaryUnitNameOfBindUnitExpr(arg1)  VhPslGetPrimaryUnitNameOfBindUnitExpr((arg1),JAG_FILE,__LINE__)
#define vhPslGetSecondaryUnitNameOfBindUnitExpr(arg1)  VhPslGetSecondaryUnitNameOfBindUnitExpr((arg1),JAG_FILE,__LINE__)
#define vhPslAvoidErrorForUnusedForallIndex(arg1) VhPslAvoidErrorForUnusedForallIndex((arg1), JAG_FILE, __LINE__)
#define vhIsAPslObj(arg1) VhIsAPslObj((arg1), JAG_FILE, __LINE__)
#define vhPslGetParametersDefinition(arg1) VhPslGetParametersDefinition((arg1), JAG_FILE, __LINE__)
#define vhAllowNullRanges(arg1) VhAllowNullRanges((arg1), JAG_FILE, __LINE__)
#define vhEnableInMemoryOutOfDateCheck(arg1) VhEnableInMemoryOutOfDateCheck((arg1), JAG_FILE, __LINE__)
#define vhCreateSignedDecLitForIntMin(arg1) VhCreateSignedDecLitForIntMin((arg1), JAG_FILE, __LINE__)
#define vhUnDefineRegionMarker(arg1, arg2) VhUnDefineRegionMarker((arg1), (arg2), JAG_FILE, __LINE__)
#define vhUnElaborateBlackBox(arg1, arg2) VhUnElaborateBlackBox((arg1), (arg2), JAG_FILE, __LINE__)
#define vhPslAllowClockAsIdentifier(arg1) VhPslAllowClockAsIdentifier((arg1), JAG_FILE, __LINE__)
#define vhPslUseFirstDeclForBitVectorValidation(arg1) VhPslUseFirstDeclForBitVectorValidation((arg1), JAG_FILE, __LINE__)
#define vhIgnoreSizeMissMatchInIfGen(arg1) VhIgnoreSizeMissMatchInIfGen((arg1), JAG_FILE, __LINE__)
#define vhRegisterIPDumpEncrypter(arg1) VhRegisterIPDumpEncrypter((arg1), JAG_FILE, __LINE__)
#define vhRegisterIPDumpDecrypter(arg1) VhRegisterIPDumpDecrypter((arg1), JAG_FILE, __LINE__)
#define vhRegisterIPDumpEncryptedTextDeallocator(arg1) VhRegisterIPDumpEncryptedTextDeallocator((arg1), JAG_FILE, __LINE__)
#define vhRegisterIPDumpDecryptedTextDeallocator(arg1) VhRegisterIPDumpDecryptedTextDeallocator((arg1), JAG_FILE, __LINE__)
#define vhGetProtectKeyValue(arg1) VhGetProtectKeyValue((arg1), JAG_FILE, __LINE__)
#define vhGetProtectKeyType(arg1) VhGetProtectKeyType((arg1), JAG_FILE, __LINE__)
#define vhRegisterEinputDecryptBufferDeallocator(arg1) VhRegisterEinputDecryptBufferDeallocator((arg1), JAG_FILE, __LINE__)
#define vhCreateDSEntForTop() VhCreateDSEntForTop(JAG_FILE, __LINE__)
#define vhDsElabOptimizationForNonInstance(arg1) VhDsElabOptimizationForNonInstance((arg1), JAG_FILE, __LINE__) 
#define vhEnable32bitTo64bitConversion(arg1) VhEnable32bitTo64bitConversion((arg1), JAG_FILE, __LINE__) 
#ifdef _DFT_HDLENGINE_
/* File offset database related PIs Start */
WIN_DLL_EXPORT    unsigned int     VhGetObjectId PROTO((vhNode namedObject, vhString file, int line));
#define vhGetObjectId(object) VhGetObjectId(object, (vhString)__FILE__, __LINE__)
WIN_DLL_EXPORT void VhSetOutputDir(const char*);
#define vhSetOutputDir(path) VhSetOutputDir(path)
/* File offset database related PIs END */
#endif
#define vhMakeDistinctElaborationWithBlockConfig(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) VhMakeDistinctElaborationWithBlockConfig((arg1), (arg2), (arg3), (arg4), (arg5), (arg6), (arg7), (arg8))
#define vhAllowImplicitRead(arg1) VhAllowImplicitRead((arg1), JAG_FILE,__LINE__)
#define vhSetFileNameForNewNode(arg1,arg2) VhSetFileNameForNewNode((arg1),(arg2),JAG_FILE,__LINE__)
#define vhEnableInternalEvalExprDeletion(arg1) VhEnableInternalEvalExprDeletion((arg1), JAG_FILE,__LINE__)
#define vhEnableRangeConstraintElab(arg1) VhEnableRangeConstraintElab((arg1), JAG_FILE,__LINE__)
#define vhHasProtectedRegion(arg1) VhHasProtectedRegion((arg1), JAG_FILE,__LINE__)
#define vhAllowOutPortRead(arg1) VhAllowOutPortRead((arg1), JAG_FILE,__LINE__)
#define vhAllowProcessAll(arg1) VhAllowProcessAll((arg1), JAG_FILE,__LINE__)
#define vhAllowCondAssign(arg1) VhAllowCondAssign((arg1), JAG_FILE,__LINE__)
#define vhEnableEvalPeakMemOpt(arg1) VhEnableEvalPeakMemOpt((arg1), JAG_FILE,__LINE__)
#define vhIsNextNodeComment(arg1) VhIsNextNodeComment((arg1),JAG_FILE,__LINE__)
#define vhKeepNextCommentInfo(arg1) VhKeepNextCommentInfo((arg1), JAG_FILE,__LINE__)
#define vhSetPrefixForArchElaborateGenerateStatement(arg1) VhSetPrefixForArchElaborateGenerateStatement((arg1), JAG_FILE,__LINE__)
#define vhGetGenerateBlockType(arg1) VhGetGenerateBlockType((arg1), JAG_FILE,__LINE__)
#define vhTreatSynthOffOnSameInAnalAndElab(arg1) VhTreatSynthOffOnSameInAnalAndElab((arg1), JAG_FILE,__LINE__)
#define vhAllowNonStaticUnaryExprInPortMap(arg1) VhAllowNonStaticUnaryExprInPortMap((arg1), JAG_FILE,__LINE__)
#define vhAllowNonStaticExprInPortMap(arg1) VhAllowNonStaticExprInPortMap((arg1), JAG_FILE,__LINE__)
#define vhFreeDependencyUnitList(arg1) VhFreeDependencyUnitList((arg1), JAG_FILE,__LINE__)
#define vhKeepNamedAssocForSingleElemRecAggrInOpenDecl(arg1) VhKeepNamedAssocForSingleElemRecAggrInOpenDecl((arg1), JAG_FILE,__LINE__)
#define vhSetLineNumberFileNameFormat(arg1) VhSetLineNumberFileNameFormat((arg1), JAG_FILE,__LINE__)
#define vhSetUserInputFileName(arg1) VhSetUserInputFileName((arg1), JAG_FILE,__LINE__)
#define vhEnableCopyDumpInLongPath(arg1) VhEnableCopyDumpInLongPath((arg1), JAG_FILE,__LINE__)
#define vhAllowMissingBindingForArchNameModule(arg1)  VhAllowMissingBindingForArchNameModule((arg1), JAG_FILE,__LINE__)
#define vhSetNewLocaleImplementation(arg1)  VhSetNewLocaleImplementation((arg1), JAG_FILE,__LINE__)
#define vhIsProcessHasAll(arg1) VhIsProcessHasAll((arg1),JAG_FILE,__LINE__)
#define vhGetForceReleaseMode(arg1) VhGetForceReleaseMode((arg1),JAG_FILE,__LINE__)
#define vhIsForceReleaseModeDefault(arg1) VhIsForceReleaseModeDefault((arg1),JAG_FILE,__LINE__)
#define vhGetForceExpr(arg1) VhGetForceExpr((arg1),JAG_FILE,__LINE__)
#define vhHasMatchingSelSig(arg1) VhHasMatchingSelSig((arg1),JAG_FILE,__LINE__)
#define vhGetSelectedExprList(arg1) VhGetSelectedExprList((arg1),JAG_FILE,__LINE__)
#define vhGetConditionalExprList(arg1) VhGetConditionalExprList((arg1),JAG_FILE,__LINE__)
#define vhResetGlobalSymTabOfDumpedUnits(arg1) VhResetGlobalSymTabOfDumpedUnits((arg1), JAG_FILE,__LINE__)
#define vhGetIsSizeMismatchOccuredDuringEval(arg1) VhGetIsSizeMismatchOccuredDuringEval((arg1), JAG_FILE,__LINE__)

#ifdef _DFT_HDLENGINE_
#define vhIsDUStructural(arg1) VhIsDUStructural((arg1),JAG_FILE,__LINE__)
#define vhIsPackFromPackInstance(arg1) VhIsPackFromPackInstance((arg1),JAG_FILE,__LINE__)
#endif

#define vhDGFreeFileBasedDependencyGraph(arg1) VhDGFreeFileBasedDependencyGraph((arg1), JAG_FILE,__LINE__)
#define vhDGFileInfoGetFileName(arg1)          VhDGFileInfoGetFileName((arg1), JAG_FILE,__LINE__)
#define vhDGFileInfoGetLibName(arg1)           VhDGFileInfoGetLibName((arg1), JAG_FILE,__LINE__)
#define vhDGFileInfoGetTokenCount(arg1)        VhDGFileInfoGetTokenCount((arg1), JAG_FILE,__LINE__)
#define vhDGFileInfoGetIncidentFileList(arg1)  VhDGFileInfoGetIncidentFileList((arg1), JAG_FILE,__LINE__)
#define vhSetPrefixForExtIdMapFileInDumpRestore(arg1,arg2)  VhSetPrefixForExtIdMapFileInDumpRestore((arg1), (arg2), JAG_FILE,__LINE__)
#define vhDGFileInfoGetUnResolvedUnitList(arg1) VhDGFileInfoGetUnResolvedUnitList((arg1), JAG_FILE,__LINE__)
#define vhDGFileInfoGetUnResolvedUnitInfo(arg1, arg2, arg3, arg4) VhDGFileInfoGetUnResolvedUnitInfo((arg1),(arg2),(arg3),(arg4),JAG_FILE,__LINE__)
#define vhDGFileInfoGetEffectiveDUCount(arg1) VhDGFileInfoGetEffectiveDUCount((arg1),JAG_FILE,__LINE__)
#define vhLogTextEdit(arg1,arg2,arg3) VhLogTextEdit((arg1),(arg2),(arg3), JAG_FILE,__LINE__) 
#define vhDumpTextEdit(arg1,arg2,arg3)    VhDumpTextEdit((arg1),(arg2),(arg3),JAG_FILE,__LINE__)
#define vhTypeGetConstraintType(arg1) VhTypeGetConstraintType((arg1),JAG_FILE,__LINE__)
#define vhGetPslVUnitBindIndList(arg1) VhGetPslVUnitBindIndList((arg1),JAG_FILE,__LINE__)
#define vhGetPslVUnitBindIndExprList(arg1) VhGetPslVUnitBindIndExprList((arg1),JAG_FILE,__LINE__)

#define vhwCreateSimpleResolutionInd(arg1) VhwCreateSimpleResolutionInd((arg1), JAG_FILE,__LINE__)
#define vhwCreateArrayResolutionInd(arg1) VhwCreateArrayResolutionInd((arg1), JAG_FILE,__LINE__)
#define vhwCreateRecordResolutionInd(arg1) VhwCreateRecordResolutionInd((arg1), JAG_FILE,__LINE__)
#define vhwCreateRecordElemResolutionInd(arg1, arg2) VhwCreateRecordElemResolutionInd((arg1), (arg2), JAG_FILE,__LINE__)
#define vhGetArrayEleSubtypeResFunc(arg1) VhGetArrayEleSubtypeResFunc((arg1), JAG_FILE,__LINE__)
#define vhGetRecordEleSubtypeResFunc(arg1) VhGetRecordEleSubtypeResFunc((arg1), JAG_FILE,__LINE__)
#define vhGetRecordElementNameExpr(arg1) VhGetRecordElementNameExpr((arg1), JAG_FILE,__LINE__)
#define vhAllowReverseRangeDecompile(arg1) VhAllowReverseRangeDecompile((arg1),JAG_FILE,__LINE__)
#define vhAllowDecompileAllLogicalOpToPlus(arg1) VhAllowDecompileAllLogicalOpToPlus((arg1),JAG_FILE,__LINE__)

#define vhIsDUStructural(arg1) VhIsDUStructural((arg1),JAG_FILE,__LINE__)
#define vhIsPackFromPackInstance(arg1) VhIsPackFromPackInstance((arg1),JAG_FILE,__LINE__)
#define vhSearchSubProgFromAttr(arg1, arg2) VhSearchSubProgFromAttr((arg1),(arg2), JAG_FILE,__LINE__)
#define vhIsTypeDependentOnGenericType(arg1) VhIsTypeDependentOnGenericType((arg1), JAG_FILE,__LINE__)
#define vhGetPackInstDeclName(arg1) VhGetPackInstDeclName((arg1), JAG_FILE,__LINE__)
#define vhIsExprFromDumpPlane(arg1) VhIsExprFromDumpPlane((arg1),JAG_FILE,__LINE__)

#define vhRegisterParseFOpenAndFCloseCallback(arg1, arg2) VhRegisterParseFOpenAndFCloseCallback((arg1), (arg2), JAG_FILE, __LINE__)
#define vhCreateNewContainerForAllScopes(arg1,arg2) VhCreateNewContainerForAllScopes((arg1), (arg2), JAG_FILE, __LINE__)
#define vhPopContainerForAllScopes(arg1) VhPopContainerForAllScopes((arg1), JAG_FILE, __LINE__)
#define vhRemoveAndGetTopFromStackForAllScope(arg1, arg2, arg3) VhRemoveAndGetTopFromStackForAllScope((arg1), (arg2), (arg3), JAG_FILE, __LINE__)
#define vhPushIntoStackForAllScope(arg1, arg2, arg3) VhPushIntoStackForAllScope((arg1), (arg2), (arg3), JAG_FILE, __LINE__)
#define vhIsRecursivelyInstantiated(arg1) VhIsRecursivelyInstantiated((arg1), JAG_FILE, __LINE__)
#define vhwCreateRangeFromIntValues(arg1, arg2) VhwCreateRangeFromIntValues(arg1, arg2)
#define vhSetConsiderInitValOfSigOneAllowedIntValue(arg1, arg2) VhSetConsiderInitValOfSigOneAllowedIntValue((arg1), (arg2), JAG_FILE, __LINE__)
#ifdef _DFT_HDLENGINE_
#define vhForceEvaluateVhDeclLit(arg1) VhForceEvaluateVhDeclLit((arg1), JAG_FILE, __LINE__)
#define vhIsUnelaboratedRangeParameterised(arg1) VhIsUnelaboratedRangeParameterised((arg1), JAG_FILE, __LINE__)
#endif
#define vhSetStdLogicVec_StdULogicVec_2008mode(arg1) VhSetStdLogicVec_StdULogicVec_2008mode((arg1), JAG_FILE, __LINE__)
#define vhSetSameDeclOverloadingAllowed(arg1) VhSetSameDeclOverloadingAllowed((arg1), JAG_FILE, __LINE__)
#define vhImportVhdlPackageToLowerNameMixedSvVh(arg1) VhImportVhdlPackageToLowerNameMixedSvVh((arg1), JAG_FILE, __LINE__)
#define vhRegisterGFileGenericValueCallBack(arg1, arg2) VhRegisterGFileGenericValueCallBack((arg1), (arg2), JAG_FILE, __LINE__)
#define vhRegisterGFilePushPopForGenerateBlock(arg1, arg2) VhRegisterGFilePushPopForGenerateBlock((arg1), (arg2), JAG_FILE, __LINE__)
#define vhRegisterNewConfigFlowCallbacks(arg1, arg2) VhRegisterNewConfigFlowCallbacks((arg1), (arg2), JAG_FILE, __LINE__)
#define vhSetIsNewConfigFlow(arg1) VhSetIsNewConfigFlow((arg1), JAG_FILE, __LINE__)
#define vhSetGFileNotEmptyFlags(arg1, arg2) VhSetGFileNotEmptyFlags((arg1), (arg2), JAG_FILE, __LINE__)
