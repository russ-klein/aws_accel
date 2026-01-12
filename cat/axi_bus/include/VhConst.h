/*==========================================

 This file and related documentation are
 proprietary and confidential to Siemens
         Copyright Siemens 2021

===========================================*/

/************************************************************************
 
    File    : VhConst.h
 
    Purpose : This file lists out the different enum types used while
                populating the JAGUAR object model.

    Enum Types :
            VhObjType
            VhAttrDensity
            VhAttrDataType
            VhOpType
            VhBoolean
            VhBaseType
            VhDirType
            VhPortType
            VhDisconnectSigType
            VhAttribSpecEntityNameListType
            VhSignalKind
            VhSubProgType
            VhAttrbType
            VhTypeType
            VhStaticType
            VhEntClassType
            VhNameType
            VhIntFlagType
            VhPureType
            VhOpCategory
            VhResolutionMethod
 
************************************************************************/
 

#ifndef __VHCONST__H
#define __VHCONST__H

#ifndef FLOATTYPE
#define FLOATTYPE double
#endif

#ifndef _preprocess_for_cgen
#include<stdio.h>
#endif

/*****
    Object type for all the Jaguar nodes. 
    NOTE: any new object type name should be added to the END of the listing
*****/

enum VhObjType {
    VHBASE ,
    VHNAMED ,
    VHDESIGNUNIT ,
    VHLIBRARYCLAUSE,
    VHUSECLAUSE,
    VHENTITY,
    VHCONFIGDECL ,
    VHPACKAGEDECL ,
    VHARCH ,
    VHPACKAGEBODY ,
    VHBLOCKCONFIG ,
    VHCONFIGITEM,
    VHCOMPCONFIG ,
    VHCOMPSPEC ,
    VHCONFIGSPEC ,
    VHENTITYASPECT ,
    VHSEQSTMNT ,
    VHWAIT ,
    VHVARASGN ,
    VHSIGASGN ,
    VHWAVEFORM ,
    VHWAVEFORMELEMENT ,
    VHPROCEDURECALL ,
    VHASSOCIATION ,
    VHIF ,
    VHCASE ,
    VHCASEALTER ,
    VHLOOP ,
    VHWHILE ,
    VHFOR ,
    VHASSERT ,
    VHREPORT ,
    VHNEXT ,
    VHEXIT ,
    VHRETURN ,
    VHNULL ,
    VHCONCSTMNT ,
    VHPROCESS ,
    VHGENERIC ,
    VHGENMAPASPECT ,
    VHPORT ,
    VHPORTMAPASPECT ,
    VHCOMPONENT ,
    VHSIGNAL ,
    VHCONSTANT ,
    VHVARIABLE ,
    VHBLOCK ,
    VHCONCPROCCALL ,
    VHINSTANCE ,
    VHCOMPINSTANCE ,
    VHENTITYINSTANCE ,
    VHCONCSIGASGN ,
    VHCONDSIGASGN ,
    VHCONDWAVEFORM ,
    VHSELSIGASGN ,
    VHSELWAVEFORM ,
    VHGENERATE ,
    VHIFGENERATE,
    VHFORGENERATE,
    VHEXPR ,
    VHOPERATOR ,
    VHUNARY ,
    VHBINARY ,
    VHFUNCCALL ,
    VHQEXPR ,
    VHTYPECONV ,
    VHSELNAME ,
    VHINDNAME ,
    VHSLICENAME ,
    VHATTRBNAME ,
    VHPHYSICALLIT ,
    VHDECLIT ,
    VHBASELIT ,
    VHENUMELEMENT,
    VHIDENUMLIT ,
    VHSTRING ,
    VHBITSTRING ,
    VHCHARLIT ,
    VHNULLLIT ,
    VHSIMPLENAME ,
    VHAGGREGATE ,
    VHALLOCATOR ,
    VHSUBPROGBODY ,
    VHSUBPROGDECL ,
    VHTYPEDECL ,
    VHTYPEDEF ,
    VHSUBTYPEDECL ,
    VHRANGE ,
    VHDISRANGE ,
    VHSUBTYPEIND ,
    VHCONSTRAINT ,
    VHINDEXCONSTRAINT,
    VHRANGECONSTRAINT ,
    VHENUMTYPE ,
    VHINTTYPE,
    VHUNCONSARRAY ,
    VHCONSARRAY ,
    VHINDSUBTYPEDEF,
    VHOTHERS ,
    VHALL ,
    VHSIGNATURE ,
    VHEXTERNAL ,
    VHFORINDEX ,
    VHOBJDECLARATION,
    VHLABEL ,
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

    /* Pre-scan node's object type.
    --- used for internal purpose. */
    VH_SCAN_ENTITY,
    VH_SCAN_PACKAGEDECL,
    VH_SCAN_CONTEXTDECL,
    VH_SCAN_CONFIGDECL,
    VH_SCAN_ARCH,
    VH_SCAN_PACKAGEBODY,
    VH64BITVALUE,

    // Following are the psl object types.
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
    // End of psl object types.
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
    VHENDLOOP,
 
    /* ---- IMPORTANT ----
       The following object type corresponds to
       the final object type for all jaguar
       objects. Hence, if any new object type
       need to be added, it should be added
       before this. DON'T ADD ANY NEW OBJECT
       TYPE AFTER THIS COMMENT.

       Along with the addition of the new object
       type in this enum, the static array
       "isATable (in PIstatic.cxx)" need to be
       updated properly. THIS IS AN ABSOLUTE MUST.
       
     */
    VH_END_OF_OBJECT_TYPES
};

/*** External Name PathName Category ***/
enum VhExternalPathName {
        VH_ERROR_PATHNAME,
        VHPACKPATHNAME,
        VHABSPATHNAME,
        VHRELPATHNAME
};

/*****
    User attribute density type.
*****/

enum VhAttrDensity {
        VH_DATA_MEMBER,
        VH_DENSE,
        VH_SPARSE
};
 
/*****
    User attribute data type.
*****/

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

/*****
    Type of the operator. 
*****/

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
    VH_COND_OP,
    VH_REL_EQ_OP,  /* VHDL-2008 support PR_14524 */
    VH_REL_NEQ_OP,
    VH_REL_GTH_OP,
    VH_REL_LTH_OP,
    VH_REL_GEQ_OP,
    VH_REL_LEQ_OP,
    VH_ERROR_OP,
    VH_SHL_OP, //VH_SHL_OP and VH_SHR_OP are added for a support under 
               //mti-compatible flag ( PR-9001 )
    VH_SHR_OP,
    VH_TOTAL_OP
};

/*****
    Boolean type used in Jaguar.
*****/

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

/*****
    BackWardCompatibility type used in Jaguar.
*****/

enum VhBackWardCompatibility{
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

/*****
    Base type used. It can be binary, unsigned binary, signed binary, octal, unsigned octal, signed octal,
    hex, unsigned hex, signed hex, decimal.
*****/

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
     
/*****
    Direction type. It can be "to", "downto".
*****/

enum VhDirType {
     VH_TO,
     VH_DOWNTO,
     VH_ERROR_DIR
};
    
/*****
    Port type of the signals, constants, variables etc.
*****/

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

/****
    Signal in Disconnect_spec can be of kind all, others
    signal_list
****/

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

/*****
    Kind of the signal, can be bus, register, guarded or not guarded.
*****/

enum VhSignalKind {
    VH_BUS,
    VH_REGISTER,
    VH_NOT_GUARDED,
    VH_ERROR_SIGTYPE
};

/*****
    Type of the subprogram, can be procedure or function.
*****/

enum VhSubProgType {
    VH_PROCEDURE_SUBPG,
    VH_FUNCTION_SUBPG,
    VH_ERROR_SUBPROGTYPE
};
    
/*****
    Type of the VHDL predefined/user-define attributes.
*****/

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
    
/*****
    Type of the VHDL type, can be integer, positive, natural etc.
*****/

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

/*****
    Static type of an expression. Can be locally static, globally static or
    non static.
*****/

enum VhStaticType {
    VH_LOCALLY_STATIC,
    VH_GLOBALLY_STATIC,
    VH_NOT_STATIC,
    VH_ERROR_STATIC_TYPE
};

/*****
    Class type of the different entities.
*****/

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

/*****
    Type of the simple name. Name can be of any type like
    pacakge name, package body name, alias name, component name etc.
*****/

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

/*****
    Interface declaration type. Can be in generic, port, function, procedure or
    normal declaration type (reset).
*****/
    
enum VhIntFlagType {
    VH_GEN,
    VH_PORT,
    VH_FUNC,
    VH_PROC,
    VH_RESET
};

/*****
    Pure type of the subprogram, can be pure or impure.
*****/

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

/*****
    Type of the operator category. Can be logical, relational, shift etc.
*****/

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
    VH_OTHERS, //VH_OTHERS is added for a support under mti-compatible 
              //flag ( PR-9001 )
    VH_TOTAL_OPCATEGORY
};

/******
    Type of different resolution method used.
******/

enum VhResolutionMethod { /* used for synopsys resolution method directive */
    VH_WIRED_OR,
    VH_WIRED_AND,
    VH_THREE_STATE,
    VH_NONE,
    VH_ERROR_RESOLUTIONMETHOD
};

/******
    Type of the region under meta comments and default is no region.
******/

enum RegionType {
    VH_TRANSLATION_OFF,
    VH_SYNTHESIS_OFF,
    VH_NO_REGION,
    VH_08_MODE_ON,
    VH_93_MODE_ON,
    VH_USER_DEFINED,
    VH_OVERRIDE_ON // PR-7977
};

enum VhRegionOrder {
    VH_DEFAULT,
    VH_LIFO,
    VH_FIFO,
    VH_RANDOM
};

/*****
    Type of the different Jaguar messages, can be info, warning, error etc.
*****/

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

/*****
    Name case type, can be upper, lower or the orginal case as it is.
****/

enum VhNameCaseType {
    VH_ASIS,
    VH_UPPER,
    VH_LOWER
};

enum scopeRegionType {
    VH_DECLRATIVE_REGION,
    VH_STATEMENT_REGION,

    /* JAG-3151 Start [ */

    VH_INTERFACE_REGION,
    VH_BEFORE_COMMA_REGION,
    VH_AFTER_COMMA_REGION,
    VH_BEFORE_END_BRACKET_REGION,

    /* ] End JAG-3151   */

    VH_OUTSIDE_SCOPE
};

enum VhSubTypeIndConstType {
    VH_UNKNOWN_CONSTRAINT,
    VH_ERROR_CONSTRAINT,
    VH_ACCESS_CONSTRAINT,
    VH_PURELY_CONSTRAINT,
    VH_PURELY_UNCONSTRAINT,
    VH_PARTIALLY_CONS_UNCONSTRAINT,
    VH_SCALOR_CONSTRAINT,
    VH_END_CONSTRAINT /* All should be added before this */
};

/* JAG-3196 Start [ */

enum VhSubTypeIndRngBoundType {
    VH_MIXED_OPEN_EXPLICIT_BOUND,
    VH_PURELY_OPEN_BOUND,
    VH_PURELY_EXPLICIT_BOUND,
    VH_UNKNOWN_BOUND
};

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
    //JAG_1991,
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

/* JAG-3025. New enum introduced to identify foreign attribute type */
enum VhForeignVhpiParseType {
    IGNORE_FOREIGN,
    VALID_FOREIGN,
    INVALID_FOREIGN
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

#ifndef _preprocess_for_cgen
typedef struct _vhEncryptionSourceInfo
{
    const char *absFileName;
    _vhEncryptionSourceInfo()
    {
        absFileName = NULL;
    }
} vhEncryptionSourceInfo;
#endif

#endif

