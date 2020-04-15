#include "iostream"

#include "antlr4-runtime.h"

#include "Python3Parser.h"
#include "Python3ParserBaseVisitor.h"
#include "Python3Lexer.h"

#include "rice/Array.hpp"
#include "rice/Class.hpp"
#include "rice/Constructor.hpp"
#include "rice/Director.hpp"

using namespace std;
using namespace Rice;
using namespace antlr4;

class ContextProxy {
public:
  ContextProxy(tree::ParseTree* orig) {
    this -> orig = orig;
  }

  tree::ParseTree* getOriginal() {
    return orig;
  }

  std::string getText() {
    return orig -> getText();
  }

  Object getChildren() {
    Array a;

    if (orig != nullptr) {
      for (auto it = orig -> children.begin(); it != orig -> children.end(); it ++) {
        a.push(ContextProxy::wrapParseTree(*it));
      }
    }

    return a;
  }

  Object getParent() {
    if (orig == nullptr) {
      return Qnil;
    }

    return ContextProxy::wrapParseTree(orig -> parent);
  }

  size_t childCount() {
    if (orig == nullptr) {
      return 0;
    }

    return orig -> children.size();
  }

private:

  static Object wrapParseTree(tree::ParseTree* node);

protected:
  tree::ParseTree* orig = nullptr;
};

class TerminalNodeProxy : public ContextProxy {
public:
  TerminalNodeProxy(tree::ParseTree* tree) : ContextProxy(tree) { }
};

class Single_inputContextProxy : public ContextProxy {
public:
  Single_inputContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object simple_stmt();
  Object compound_stmt();
  Object NEWLINE();
};

class Simple_stmtContextProxy : public ContextProxy {
public:
  Simple_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object small_stmt();
  Object small_stmtAt(size_t i);
  Object NEWLINE();
  Object SEMI_COLON();
  Object SEMI_COLONAt(size_t i);
};

class Compound_stmtContextProxy : public ContextProxy {
public:
  Compound_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object if_stmt();
  Object while_stmt();
  Object for_stmt();
  Object try_stmt();
  Object with_stmt();
  Object funcdef();
  Object classdef();
  Object decorated();
  Object async_stmt();

};

class File_inputContextProxy : public ContextProxy {
public:
  File_inputContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object stmt();
  Object stmtAt(size_t i);
  Object EOF();
  Object NEWLINE();
  Object NEWLINEAt(size_t i);
};

class StmtContextProxy : public ContextProxy {
public:
  StmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object simple_stmt();
  Object compound_stmt();

};

class Eval_inputContextProxy : public ContextProxy {
public:
  Eval_inputContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object testlist();
  Object EOF();
  Object NEWLINE();
  Object NEWLINEAt(size_t i);
};

class TestlistContextProxy : public ContextProxy {
public:
  TestlistContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class DecoratorContextProxy : public ContextProxy {
public:
  DecoratorContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object dotted_name();
  Object arglist();
  Object AT();
  Object NEWLINE();
  Object OPEN_PAREN();
  Object CLOSE_PAREN();
};

class Dotted_nameContextProxy : public ContextProxy {
public:
  Dotted_nameContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object NAME();
  Object NAMEAt(size_t i);
  Object DOT();
  Object DOTAt(size_t i);
};

class ArglistContextProxy : public ContextProxy {
public:
  ArglistContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object argument();
  Object argumentAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class DecoratorsContextProxy : public ContextProxy {
public:
  DecoratorsContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object decorator();
  Object decoratorAt(size_t i);

};

class DecoratedContextProxy : public ContextProxy {
public:
  DecoratedContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object decorators();
  Object classdef();
  Object funcdef();
  Object async_funcdef();

};

class ClassdefContextProxy : public ContextProxy {
public:
  ClassdefContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object suite();
  Object arglist();
  Object CLASS();
  Object NAME();
  Object COLON();
  Object OPEN_PAREN();
  Object CLOSE_PAREN();
};

class FuncdefContextProxy : public ContextProxy {
public:
  FuncdefContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object parameters();
  Object suite();
  Object test();
  Object DEF();
  Object NAME();
  Object COLON();
  Object ARROW();
};

class Async_funcdefContextProxy : public ContextProxy {
public:
  Async_funcdefContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object funcdef();
  Object ASYNC();
};

class ParametersContextProxy : public ContextProxy {
public:
  ParametersContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object typedargslist();
  Object OPEN_PAREN();
  Object CLOSE_PAREN();
};

class SuiteContextProxy : public ContextProxy {
public:
  SuiteContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object simple_stmt();
  Object stmt();
  Object stmtAt(size_t i);
  Object NEWLINE();
  Object INDENT();
  Object DEDENT();
};

class TestContextProxy : public ContextProxy {
public:
  TestContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object or_test();
  Object or_testAt(size_t i);
  Object test();
  Object lambdef();
  Object IF();
  Object ELSE();
};

class TypedargslistContextProxy : public ContextProxy {
public:
  TypedargslistContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object tfpdef();
  Object tfpdefAt(size_t i);
  Object test();
  Object testAt(size_t i);
  Object STAR();
  Object POWER();
  Object ASSIGN();
  Object ASSIGNAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class TfpdefContextProxy : public ContextProxy {
public:
  TfpdefContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object NAME();
  Object COLON();
};

class VarargslistContextProxy : public ContextProxy {
public:
  VarargslistContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object vfpdef();
  Object vfpdefAt(size_t i);
  Object test();
  Object testAt(size_t i);
  Object STAR();
  Object POWER();
  Object ASSIGN();
  Object ASSIGNAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class VfpdefContextProxy : public ContextProxy {
public:
  VfpdefContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object NAME();
};

class Small_stmtContextProxy : public ContextProxy {
public:
  Small_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object expr_stmt();
  Object del_stmt();
  Object pass_stmt();
  Object flow_stmt();
  Object import_stmt();
  Object global_stmt();
  Object nonlocal_stmt();
  Object assert_stmt();

};

class Expr_stmtContextProxy : public ContextProxy {
public:
  Expr_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object testlist_star_expr();
  Object testlist_star_exprAt(size_t i);
  Object annassign();
  Object augassign();
  Object yield_expr();
  Object yield_exprAt(size_t i);
  Object testlist();
  Object ASSIGN();
  Object ASSIGNAt(size_t i);
};

class Del_stmtContextProxy : public ContextProxy {
public:
  Del_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object exprlist();
  Object DEL();
};

class Pass_stmtContextProxy : public ContextProxy {
public:
  Pass_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object PASS();
};

class Flow_stmtContextProxy : public ContextProxy {
public:
  Flow_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object break_stmt();
  Object continue_stmt();
  Object return_stmt();
  Object raise_stmt();
  Object yield_stmt();

};

class Import_stmtContextProxy : public ContextProxy {
public:
  Import_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object import_name();
  Object import_from();

};

class Global_stmtContextProxy : public ContextProxy {
public:
  Global_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object GLOBAL();
  Object NAME();
  Object NAMEAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Nonlocal_stmtContextProxy : public ContextProxy {
public:
  Nonlocal_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object NONLOCAL();
  Object NAME();
  Object NAMEAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Assert_stmtContextProxy : public ContextProxy {
public:
  Assert_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object ASSERT();
  Object COMMA();
};

class Testlist_star_exprContextProxy : public ContextProxy {
public:
  Testlist_star_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object star_expr();
  Object star_exprAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class AnnassignContextProxy : public ContextProxy {
public:
  AnnassignContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object COLON();
  Object ASSIGN();
};

class AugassignContextProxy : public ContextProxy {
public:
  AugassignContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object ADD_ASSIGN();
  Object SUB_ASSIGN();
  Object MULT_ASSIGN();
  Object AT_ASSIGN();
  Object DIV_ASSIGN();
  Object MOD_ASSIGN();
  Object AND_ASSIGN();
  Object OR_ASSIGN();
  Object XOR_ASSIGN();
  Object LEFT_SHIFT_ASSIGN();
  Object RIGHT_SHIFT_ASSIGN();
  Object POWER_ASSIGN();
  Object IDIV_ASSIGN();
};

class Yield_exprContextProxy : public ContextProxy {
public:
  Yield_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object yield_arg();
  Object YIELD();
};

class Star_exprContextProxy : public ContextProxy {
public:
  Star_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object expr();
  Object STAR();
};

class ExprlistContextProxy : public ContextProxy {
public:
  ExprlistContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object expr();
  Object exprAt(size_t i);
  Object star_expr();
  Object star_exprAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Break_stmtContextProxy : public ContextProxy {
public:
  Break_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object BREAK();
};

class Continue_stmtContextProxy : public ContextProxy {
public:
  Continue_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object CONTINUE();
};

class Return_stmtContextProxy : public ContextProxy {
public:
  Return_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object testlist();
  Object RETURN();
};

class Raise_stmtContextProxy : public ContextProxy {
public:
  Raise_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object RAISE();
  Object FROM();
};

class Yield_stmtContextProxy : public ContextProxy {
public:
  Yield_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object yield_expr();

};

class Import_nameContextProxy : public ContextProxy {
public:
  Import_nameContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object dotted_as_names();
  Object IMPORT();
};

class Import_fromContextProxy : public ContextProxy {
public:
  Import_fromContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object dotted_name();
  Object import_as_names();
  Object FROM();
  Object IMPORT();
  Object STAR();
  Object OPEN_PAREN();
  Object CLOSE_PAREN();
  Object DOT();
  Object DOTAt(size_t i);
  Object ELLIPSIS();
  Object ELLIPSISAt(size_t i);
};

class Dotted_as_namesContextProxy : public ContextProxy {
public:
  Dotted_as_namesContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object dotted_as_name();
  Object dotted_as_nameAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Import_as_namesContextProxy : public ContextProxy {
public:
  Import_as_namesContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object import_as_name();
  Object import_as_nameAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Import_as_nameContextProxy : public ContextProxy {
public:
  Import_as_nameContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object NAME();
  Object NAMEAt(size_t i);
  Object AS();
};

class Dotted_as_nameContextProxy : public ContextProxy {
public:
  Dotted_as_nameContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object dotted_name();
  Object AS();
  Object NAME();
};

class If_stmtContextProxy : public ContextProxy {
public:
  If_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object suite();
  Object suiteAt(size_t i);
  Object IF();
  Object COLON();
  Object COLONAt(size_t i);
  Object ELIF();
  Object ELIFAt(size_t i);
  Object ELSE();
};

class While_stmtContextProxy : public ContextProxy {
public:
  While_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object suite();
  Object suiteAt(size_t i);
  Object WHILE();
  Object COLON();
  Object COLONAt(size_t i);
  Object ELSE();
};

class For_stmtContextProxy : public ContextProxy {
public:
  For_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object exprlist();
  Object testlist();
  Object suite();
  Object suiteAt(size_t i);
  Object FOR();
  Object IN();
  Object COLON();
  Object COLONAt(size_t i);
  Object ELSE();
};

class Try_stmtContextProxy : public ContextProxy {
public:
  Try_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object suite();
  Object suiteAt(size_t i);
  Object except_clause();
  Object except_clauseAt(size_t i);
  Object TRY();
  Object COLON();
  Object COLONAt(size_t i);
  Object FINALLY();
  Object ELSE();
};

class With_stmtContextProxy : public ContextProxy {
public:
  With_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object with_item();
  Object with_itemAt(size_t i);
  Object suite();
  Object WITH();
  Object COLON();
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Async_stmtContextProxy : public ContextProxy {
public:
  Async_stmtContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object funcdef();
  Object with_stmt();
  Object for_stmt();
  Object ASYNC();
};

class Except_clauseContextProxy : public ContextProxy {
public:
  Except_clauseContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object EXCEPT();
  Object AS();
  Object NAME();
};

class With_itemContextProxy : public ContextProxy {
public:
  With_itemContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object expr();
  Object AS();
};

class ExprContextProxy : public ContextProxy {
public:
  ExprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object xor_expr();
  Object xor_exprAt(size_t i);
  Object OR_OP();
  Object OR_OPAt(size_t i);
};

class Or_testContextProxy : public ContextProxy {
public:
  Or_testContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object and_test();
  Object and_testAt(size_t i);
  Object OR();
  Object ORAt(size_t i);
};

class LambdefContextProxy : public ContextProxy {
public:
  LambdefContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object varargslist();
  Object LAMBDA();
  Object COLON();
};

class Test_nocondContextProxy : public ContextProxy {
public:
  Test_nocondContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object or_test();
  Object lambdef_nocond();

};

class Lambdef_nocondContextProxy : public ContextProxy {
public:
  Lambdef_nocondContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test_nocond();
  Object varargslist();
  Object LAMBDA();
  Object COLON();
};

class And_testContextProxy : public ContextProxy {
public:
  And_testContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object not_test();
  Object not_testAt(size_t i);
  Object AND();
  Object ANDAt(size_t i);
};

class Not_testContextProxy : public ContextProxy {
public:
  Not_testContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object not_test();
  Object comparison();
  Object NOT();
};

class ComparisonContextProxy : public ContextProxy {
public:
  ComparisonContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object expr();
  Object exprAt(size_t i);
  Object comp_op();
  Object comp_opAt(size_t i);

};

class Comp_opContextProxy : public ContextProxy {
public:
  Comp_opContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object LESS_THAN();
  Object GREATER_THAN();
  Object EQUALS();
  Object GT_EQ();
  Object LT_EQ();
  Object NOT_EQ_1();
  Object NOT_EQ_2();
  Object IN();
  Object NOT();
  Object IS();
};

class Xor_exprContextProxy : public ContextProxy {
public:
  Xor_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object and_expr();
  Object and_exprAt(size_t i);
  Object XOR();
  Object XORAt(size_t i);
};

class And_exprContextProxy : public ContextProxy {
public:
  And_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object shift_expr();
  Object shift_exprAt(size_t i);
  Object AND_OP();
  Object AND_OPAt(size_t i);
};

class Shift_exprContextProxy : public ContextProxy {
public:
  Shift_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object arith_expr();
  Object arith_exprAt(size_t i);
  Object LEFT_SHIFT();
  Object LEFT_SHIFTAt(size_t i);
  Object RIGHT_SHIFT();
  Object RIGHT_SHIFTAt(size_t i);
};

class Arith_exprContextProxy : public ContextProxy {
public:
  Arith_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object term();
  Object termAt(size_t i);
  Object ADD();
  Object ADDAt(size_t i);
  Object MINUS();
  Object MINUSAt(size_t i);
};

class TermContextProxy : public ContextProxy {
public:
  TermContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object factor();
  Object factorAt(size_t i);
  Object STAR();
  Object STARAt(size_t i);
  Object AT();
  Object ATAt(size_t i);
  Object DIV();
  Object DIVAt(size_t i);
  Object MOD();
  Object MODAt(size_t i);
  Object IDIV();
  Object IDIVAt(size_t i);
};

class FactorContextProxy : public ContextProxy {
public:
  FactorContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object factor();
  Object power();
  Object ADD();
  Object MINUS();
  Object NOT_OP();
};

class PowerContextProxy : public ContextProxy {
public:
  PowerContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object atom_expr();
  Object factor();
  Object POWER();
};

class Atom_exprContextProxy : public ContextProxy {
public:
  Atom_exprContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object atom();
  Object trailer();
  Object trailerAt(size_t i);
  Object AWAIT();
};

class AtomContextProxy : public ContextProxy {
public:
  AtomContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object yield_expr();
  Object testlist_comp();
  Object dictorsetmaker();
  Object OPEN_PAREN();
  Object CLOSE_PAREN();
  Object OPEN_BRACK();
  Object CLOSE_BRACK();
  Object OPEN_BRACE();
  Object CLOSE_BRACE();
  Object NAME();
  Object NUMBER();
  Object ELLIPSIS();
  Object NONE();
  Object TRUE();
  Object FALSE();
  Object STRING();
  Object STRINGAt(size_t i);
};

class TrailerContextProxy : public ContextProxy {
public:
  TrailerContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object arglist();
  Object subscriptlist();
  Object OPEN_PAREN();
  Object CLOSE_PAREN();
  Object OPEN_BRACK();
  Object CLOSE_BRACK();
  Object DOT();
  Object NAME();
};

class Testlist_compContextProxy : public ContextProxy {
public:
  Testlist_compContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object star_expr();
  Object star_exprAt(size_t i);
  Object comp_for();
  Object COMMA();
  Object COMMAAt(size_t i);
};

class DictorsetmakerContextProxy : public ContextProxy {
public:
  DictorsetmakerContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object expr();
  Object exprAt(size_t i);
  Object comp_for();
  Object star_expr();
  Object star_exprAt(size_t i);
  Object COLON();
  Object COLONAt(size_t i);
  Object POWER();
  Object POWERAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class Comp_forContextProxy : public ContextProxy {
public:
  Comp_forContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object exprlist();
  Object or_test();
  Object comp_iter();
  Object FOR();
  Object IN();
  Object ASYNC();
};

class SubscriptlistContextProxy : public ContextProxy {
public:
  SubscriptlistContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object subscript();
  Object subscriptAt(size_t i);
  Object COMMA();
  Object COMMAAt(size_t i);
};

class SubscriptContextProxy : public ContextProxy {
public:
  SubscriptContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object sliceop();
  Object COLON();
};

class SliceopContextProxy : public ContextProxy {
public:
  SliceopContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object COLON();
};

class ArgumentContextProxy : public ContextProxy {
public:
  ArgumentContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testAt(size_t i);
  Object comp_for();
  Object ASSIGN();
  Object POWER();
  Object STAR();
};

class Comp_iterContextProxy : public ContextProxy {
public:
  Comp_iterContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object comp_for();
  Object comp_if();

};

class Comp_ifContextProxy : public ContextProxy {
public:
  Comp_ifContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test_nocond();
  Object comp_iter();
  Object IF();
};

class Encoding_declContextProxy : public ContextProxy {
public:
  Encoding_declContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};

  Object NAME();
};

class Yield_argContextProxy : public ContextProxy {
public:
  Yield_argContextProxy(tree::ParseTree* ctx) : ContextProxy(ctx) {};
  Object test();
  Object testlist();
  Object FROM();
};


Class rb_cSingle_inputContext;
Class rb_cSimple_stmtContext;
Class rb_cCompound_stmtContext;
Class rb_cFile_inputContext;
Class rb_cStmtContext;
Class rb_cEval_inputContext;
Class rb_cTestlistContext;
Class rb_cDecoratorContext;
Class rb_cDotted_nameContext;
Class rb_cArglistContext;
Class rb_cDecoratorsContext;
Class rb_cDecoratedContext;
Class rb_cClassdefContext;
Class rb_cFuncdefContext;
Class rb_cAsync_funcdefContext;
Class rb_cParametersContext;
Class rb_cSuiteContext;
Class rb_cTestContext;
Class rb_cTypedargslistContext;
Class rb_cTfpdefContext;
Class rb_cVarargslistContext;
Class rb_cVfpdefContext;
Class rb_cSmall_stmtContext;
Class rb_cExpr_stmtContext;
Class rb_cDel_stmtContext;
Class rb_cPass_stmtContext;
Class rb_cFlow_stmtContext;
Class rb_cImport_stmtContext;
Class rb_cGlobal_stmtContext;
Class rb_cNonlocal_stmtContext;
Class rb_cAssert_stmtContext;
Class rb_cTestlist_star_exprContext;
Class rb_cAnnassignContext;
Class rb_cAugassignContext;
Class rb_cYield_exprContext;
Class rb_cStar_exprContext;
Class rb_cExprlistContext;
Class rb_cBreak_stmtContext;
Class rb_cContinue_stmtContext;
Class rb_cReturn_stmtContext;
Class rb_cRaise_stmtContext;
Class rb_cYield_stmtContext;
Class rb_cImport_nameContext;
Class rb_cImport_fromContext;
Class rb_cDotted_as_namesContext;
Class rb_cImport_as_namesContext;
Class rb_cImport_as_nameContext;
Class rb_cDotted_as_nameContext;
Class rb_cIf_stmtContext;
Class rb_cWhile_stmtContext;
Class rb_cFor_stmtContext;
Class rb_cTry_stmtContext;
Class rb_cWith_stmtContext;
Class rb_cAsync_stmtContext;
Class rb_cExcept_clauseContext;
Class rb_cWith_itemContext;
Class rb_cExprContext;
Class rb_cOr_testContext;
Class rb_cLambdefContext;
Class rb_cTest_nocondContext;
Class rb_cLambdef_nocondContext;
Class rb_cAnd_testContext;
Class rb_cNot_testContext;
Class rb_cComparisonContext;
Class rb_cComp_opContext;
Class rb_cXor_exprContext;
Class rb_cAnd_exprContext;
Class rb_cShift_exprContext;
Class rb_cArith_exprContext;
Class rb_cTermContext;
Class rb_cFactorContext;
Class rb_cPowerContext;
Class rb_cAtom_exprContext;
Class rb_cAtomContext;
Class rb_cTrailerContext;
Class rb_cTestlist_compContext;
Class rb_cDictorsetmakerContext;
Class rb_cComp_forContext;
Class rb_cSubscriptlistContext;
Class rb_cSubscriptContext;
Class rb_cSliceopContext;
Class rb_cArgumentContext;
Class rb_cComp_iterContext;
Class rb_cComp_ifContext;
Class rb_cEncoding_declContext;
Class rb_cYield_argContext;
Class rb_cToken;
Class rb_cParser;
Class rb_cParseTree;
Class rb_cTerminalNode;
Class rb_cTerminalNodeImpl;

template <>
Object to_ruby<Token*>(Token* const &x) {
  if (!x) return Nil;
  return Data_Object<Token>(x, rb_cToken, nullptr, nullptr);
}

template <>
Object to_ruby<tree::ParseTree*>(tree::ParseTree* const &x) {
  if (!x) return Nil;
  return Data_Object<tree::ParseTree>(x, rb_cParseTree, nullptr, nullptr);
}

template <>
Object to_ruby<tree::TerminalNode*>(tree::TerminalNode* const &x) {
  if (!x) return Nil;
  return Data_Object<tree::TerminalNode>(x, rb_cTerminalNode, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Single_inputContext*>(Python3Parser::Single_inputContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Single_inputContext>(x, rb_cSingle_inputContext, nullptr, nullptr);
}

template <>
Object to_ruby<Single_inputContextProxy*>(Single_inputContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Single_inputContextProxy>(x, rb_cSingle_inputContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Simple_stmtContext*>(Python3Parser::Simple_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Simple_stmtContext>(x, rb_cSimple_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Simple_stmtContextProxy*>(Simple_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Simple_stmtContextProxy>(x, rb_cSimple_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Compound_stmtContext*>(Python3Parser::Compound_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Compound_stmtContext>(x, rb_cCompound_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Compound_stmtContextProxy*>(Compound_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Compound_stmtContextProxy>(x, rb_cCompound_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::File_inputContext*>(Python3Parser::File_inputContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::File_inputContext>(x, rb_cFile_inputContext, nullptr, nullptr);
}

template <>
Object to_ruby<File_inputContextProxy*>(File_inputContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<File_inputContextProxy>(x, rb_cFile_inputContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::StmtContext*>(Python3Parser::StmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::StmtContext>(x, rb_cStmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<StmtContextProxy*>(StmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<StmtContextProxy>(x, rb_cStmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Eval_inputContext*>(Python3Parser::Eval_inputContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Eval_inputContext>(x, rb_cEval_inputContext, nullptr, nullptr);
}

template <>
Object to_ruby<Eval_inputContextProxy*>(Eval_inputContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Eval_inputContextProxy>(x, rb_cEval_inputContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::TestlistContext*>(Python3Parser::TestlistContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::TestlistContext>(x, rb_cTestlistContext, nullptr, nullptr);
}

template <>
Object to_ruby<TestlistContextProxy*>(TestlistContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<TestlistContextProxy>(x, rb_cTestlistContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::DecoratorContext*>(Python3Parser::DecoratorContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::DecoratorContext>(x, rb_cDecoratorContext, nullptr, nullptr);
}

template <>
Object to_ruby<DecoratorContextProxy*>(DecoratorContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<DecoratorContextProxy>(x, rb_cDecoratorContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Dotted_nameContext*>(Python3Parser::Dotted_nameContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Dotted_nameContext>(x, rb_cDotted_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Dotted_nameContextProxy*>(Dotted_nameContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Dotted_nameContextProxy>(x, rb_cDotted_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ArglistContext*>(Python3Parser::ArglistContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ArglistContext>(x, rb_cArglistContext, nullptr, nullptr);
}

template <>
Object to_ruby<ArglistContextProxy*>(ArglistContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ArglistContextProxy>(x, rb_cArglistContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::DecoratorsContext*>(Python3Parser::DecoratorsContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::DecoratorsContext>(x, rb_cDecoratorsContext, nullptr, nullptr);
}

template <>
Object to_ruby<DecoratorsContextProxy*>(DecoratorsContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<DecoratorsContextProxy>(x, rb_cDecoratorsContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::DecoratedContext*>(Python3Parser::DecoratedContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::DecoratedContext>(x, rb_cDecoratedContext, nullptr, nullptr);
}

template <>
Object to_ruby<DecoratedContextProxy*>(DecoratedContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<DecoratedContextProxy>(x, rb_cDecoratedContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ClassdefContext*>(Python3Parser::ClassdefContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ClassdefContext>(x, rb_cClassdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<ClassdefContextProxy*>(ClassdefContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ClassdefContextProxy>(x, rb_cClassdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::FuncdefContext*>(Python3Parser::FuncdefContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::FuncdefContext>(x, rb_cFuncdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<FuncdefContextProxy*>(FuncdefContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<FuncdefContextProxy>(x, rb_cFuncdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Async_funcdefContext*>(Python3Parser::Async_funcdefContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Async_funcdefContext>(x, rb_cAsync_funcdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Async_funcdefContextProxy*>(Async_funcdefContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Async_funcdefContextProxy>(x, rb_cAsync_funcdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ParametersContext*>(Python3Parser::ParametersContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ParametersContext>(x, rb_cParametersContext, nullptr, nullptr);
}

template <>
Object to_ruby<ParametersContextProxy*>(ParametersContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ParametersContextProxy>(x, rb_cParametersContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::SuiteContext*>(Python3Parser::SuiteContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::SuiteContext>(x, rb_cSuiteContext, nullptr, nullptr);
}

template <>
Object to_ruby<SuiteContextProxy*>(SuiteContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<SuiteContextProxy>(x, rb_cSuiteContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::TestContext*>(Python3Parser::TestContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::TestContext>(x, rb_cTestContext, nullptr, nullptr);
}

template <>
Object to_ruby<TestContextProxy*>(TestContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<TestContextProxy>(x, rb_cTestContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::TypedargslistContext*>(Python3Parser::TypedargslistContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::TypedargslistContext>(x, rb_cTypedargslistContext, nullptr, nullptr);
}

template <>
Object to_ruby<TypedargslistContextProxy*>(TypedargslistContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<TypedargslistContextProxy>(x, rb_cTypedargslistContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::TfpdefContext*>(Python3Parser::TfpdefContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::TfpdefContext>(x, rb_cTfpdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<TfpdefContextProxy*>(TfpdefContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<TfpdefContextProxy>(x, rb_cTfpdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::VarargslistContext*>(Python3Parser::VarargslistContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::VarargslistContext>(x, rb_cVarargslistContext, nullptr, nullptr);
}

template <>
Object to_ruby<VarargslistContextProxy*>(VarargslistContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<VarargslistContextProxy>(x, rb_cVarargslistContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::VfpdefContext*>(Python3Parser::VfpdefContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::VfpdefContext>(x, rb_cVfpdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<VfpdefContextProxy*>(VfpdefContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<VfpdefContextProxy>(x, rb_cVfpdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Small_stmtContext*>(Python3Parser::Small_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Small_stmtContext>(x, rb_cSmall_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Small_stmtContextProxy*>(Small_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Small_stmtContextProxy>(x, rb_cSmall_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Expr_stmtContext*>(Python3Parser::Expr_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Expr_stmtContext>(x, rb_cExpr_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Expr_stmtContextProxy*>(Expr_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Expr_stmtContextProxy>(x, rb_cExpr_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Del_stmtContext*>(Python3Parser::Del_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Del_stmtContext>(x, rb_cDel_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Del_stmtContextProxy*>(Del_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Del_stmtContextProxy>(x, rb_cDel_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Pass_stmtContext*>(Python3Parser::Pass_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Pass_stmtContext>(x, rb_cPass_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Pass_stmtContextProxy*>(Pass_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Pass_stmtContextProxy>(x, rb_cPass_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Flow_stmtContext*>(Python3Parser::Flow_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Flow_stmtContext>(x, rb_cFlow_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Flow_stmtContextProxy*>(Flow_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Flow_stmtContextProxy>(x, rb_cFlow_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Import_stmtContext*>(Python3Parser::Import_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Import_stmtContext>(x, rb_cImport_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Import_stmtContextProxy*>(Import_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Import_stmtContextProxy>(x, rb_cImport_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Global_stmtContext*>(Python3Parser::Global_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Global_stmtContext>(x, rb_cGlobal_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Global_stmtContextProxy*>(Global_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Global_stmtContextProxy>(x, rb_cGlobal_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Nonlocal_stmtContext*>(Python3Parser::Nonlocal_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Nonlocal_stmtContext>(x, rb_cNonlocal_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Nonlocal_stmtContextProxy*>(Nonlocal_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Nonlocal_stmtContextProxy>(x, rb_cNonlocal_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Assert_stmtContext*>(Python3Parser::Assert_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Assert_stmtContext>(x, rb_cAssert_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Assert_stmtContextProxy*>(Assert_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Assert_stmtContextProxy>(x, rb_cAssert_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Testlist_star_exprContext*>(Python3Parser::Testlist_star_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Testlist_star_exprContext>(x, rb_cTestlist_star_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Testlist_star_exprContextProxy*>(Testlist_star_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Testlist_star_exprContextProxy>(x, rb_cTestlist_star_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::AnnassignContext*>(Python3Parser::AnnassignContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::AnnassignContext>(x, rb_cAnnassignContext, nullptr, nullptr);
}

template <>
Object to_ruby<AnnassignContextProxy*>(AnnassignContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<AnnassignContextProxy>(x, rb_cAnnassignContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::AugassignContext*>(Python3Parser::AugassignContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::AugassignContext>(x, rb_cAugassignContext, nullptr, nullptr);
}

template <>
Object to_ruby<AugassignContextProxy*>(AugassignContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<AugassignContextProxy>(x, rb_cAugassignContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Yield_exprContext*>(Python3Parser::Yield_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Yield_exprContext>(x, rb_cYield_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Yield_exprContextProxy*>(Yield_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Yield_exprContextProxy>(x, rb_cYield_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Star_exprContext*>(Python3Parser::Star_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Star_exprContext>(x, rb_cStar_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Star_exprContextProxy*>(Star_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Star_exprContextProxy>(x, rb_cStar_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ExprlistContext*>(Python3Parser::ExprlistContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ExprlistContext>(x, rb_cExprlistContext, nullptr, nullptr);
}

template <>
Object to_ruby<ExprlistContextProxy*>(ExprlistContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ExprlistContextProxy>(x, rb_cExprlistContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Break_stmtContext*>(Python3Parser::Break_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Break_stmtContext>(x, rb_cBreak_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Break_stmtContextProxy*>(Break_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Break_stmtContextProxy>(x, rb_cBreak_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Continue_stmtContext*>(Python3Parser::Continue_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Continue_stmtContext>(x, rb_cContinue_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Continue_stmtContextProxy*>(Continue_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Continue_stmtContextProxy>(x, rb_cContinue_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Return_stmtContext*>(Python3Parser::Return_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Return_stmtContext>(x, rb_cReturn_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Return_stmtContextProxy*>(Return_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Return_stmtContextProxy>(x, rb_cReturn_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Raise_stmtContext*>(Python3Parser::Raise_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Raise_stmtContext>(x, rb_cRaise_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Raise_stmtContextProxy*>(Raise_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Raise_stmtContextProxy>(x, rb_cRaise_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Yield_stmtContext*>(Python3Parser::Yield_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Yield_stmtContext>(x, rb_cYield_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Yield_stmtContextProxy*>(Yield_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Yield_stmtContextProxy>(x, rb_cYield_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Import_nameContext*>(Python3Parser::Import_nameContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Import_nameContext>(x, rb_cImport_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Import_nameContextProxy*>(Import_nameContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Import_nameContextProxy>(x, rb_cImport_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Import_fromContext*>(Python3Parser::Import_fromContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Import_fromContext>(x, rb_cImport_fromContext, nullptr, nullptr);
}

template <>
Object to_ruby<Import_fromContextProxy*>(Import_fromContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Import_fromContextProxy>(x, rb_cImport_fromContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Dotted_as_namesContext*>(Python3Parser::Dotted_as_namesContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Dotted_as_namesContext>(x, rb_cDotted_as_namesContext, nullptr, nullptr);
}

template <>
Object to_ruby<Dotted_as_namesContextProxy*>(Dotted_as_namesContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Dotted_as_namesContextProxy>(x, rb_cDotted_as_namesContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Import_as_namesContext*>(Python3Parser::Import_as_namesContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Import_as_namesContext>(x, rb_cImport_as_namesContext, nullptr, nullptr);
}

template <>
Object to_ruby<Import_as_namesContextProxy*>(Import_as_namesContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Import_as_namesContextProxy>(x, rb_cImport_as_namesContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Import_as_nameContext*>(Python3Parser::Import_as_nameContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Import_as_nameContext>(x, rb_cImport_as_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Import_as_nameContextProxy*>(Import_as_nameContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Import_as_nameContextProxy>(x, rb_cImport_as_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Dotted_as_nameContext*>(Python3Parser::Dotted_as_nameContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Dotted_as_nameContext>(x, rb_cDotted_as_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Dotted_as_nameContextProxy*>(Dotted_as_nameContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Dotted_as_nameContextProxy>(x, rb_cDotted_as_nameContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::If_stmtContext*>(Python3Parser::If_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::If_stmtContext>(x, rb_cIf_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<If_stmtContextProxy*>(If_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<If_stmtContextProxy>(x, rb_cIf_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::While_stmtContext*>(Python3Parser::While_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::While_stmtContext>(x, rb_cWhile_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<While_stmtContextProxy*>(While_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<While_stmtContextProxy>(x, rb_cWhile_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::For_stmtContext*>(Python3Parser::For_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::For_stmtContext>(x, rb_cFor_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<For_stmtContextProxy*>(For_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<For_stmtContextProxy>(x, rb_cFor_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Try_stmtContext*>(Python3Parser::Try_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Try_stmtContext>(x, rb_cTry_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Try_stmtContextProxy*>(Try_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Try_stmtContextProxy>(x, rb_cTry_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::With_stmtContext*>(Python3Parser::With_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::With_stmtContext>(x, rb_cWith_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<With_stmtContextProxy*>(With_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<With_stmtContextProxy>(x, rb_cWith_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Async_stmtContext*>(Python3Parser::Async_stmtContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Async_stmtContext>(x, rb_cAsync_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Async_stmtContextProxy*>(Async_stmtContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Async_stmtContextProxy>(x, rb_cAsync_stmtContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Except_clauseContext*>(Python3Parser::Except_clauseContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Except_clauseContext>(x, rb_cExcept_clauseContext, nullptr, nullptr);
}

template <>
Object to_ruby<Except_clauseContextProxy*>(Except_clauseContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Except_clauseContextProxy>(x, rb_cExcept_clauseContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::With_itemContext*>(Python3Parser::With_itemContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::With_itemContext>(x, rb_cWith_itemContext, nullptr, nullptr);
}

template <>
Object to_ruby<With_itemContextProxy*>(With_itemContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<With_itemContextProxy>(x, rb_cWith_itemContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ExprContext*>(Python3Parser::ExprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ExprContext>(x, rb_cExprContext, nullptr, nullptr);
}

template <>
Object to_ruby<ExprContextProxy*>(ExprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ExprContextProxy>(x, rb_cExprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Or_testContext*>(Python3Parser::Or_testContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Or_testContext>(x, rb_cOr_testContext, nullptr, nullptr);
}

template <>
Object to_ruby<Or_testContextProxy*>(Or_testContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Or_testContextProxy>(x, rb_cOr_testContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::LambdefContext*>(Python3Parser::LambdefContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::LambdefContext>(x, rb_cLambdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<LambdefContextProxy*>(LambdefContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<LambdefContextProxy>(x, rb_cLambdefContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Test_nocondContext*>(Python3Parser::Test_nocondContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Test_nocondContext>(x, rb_cTest_nocondContext, nullptr, nullptr);
}

template <>
Object to_ruby<Test_nocondContextProxy*>(Test_nocondContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Test_nocondContextProxy>(x, rb_cTest_nocondContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Lambdef_nocondContext*>(Python3Parser::Lambdef_nocondContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Lambdef_nocondContext>(x, rb_cLambdef_nocondContext, nullptr, nullptr);
}

template <>
Object to_ruby<Lambdef_nocondContextProxy*>(Lambdef_nocondContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Lambdef_nocondContextProxy>(x, rb_cLambdef_nocondContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::And_testContext*>(Python3Parser::And_testContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::And_testContext>(x, rb_cAnd_testContext, nullptr, nullptr);
}

template <>
Object to_ruby<And_testContextProxy*>(And_testContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<And_testContextProxy>(x, rb_cAnd_testContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Not_testContext*>(Python3Parser::Not_testContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Not_testContext>(x, rb_cNot_testContext, nullptr, nullptr);
}

template <>
Object to_ruby<Not_testContextProxy*>(Not_testContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Not_testContextProxy>(x, rb_cNot_testContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ComparisonContext*>(Python3Parser::ComparisonContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ComparisonContext>(x, rb_cComparisonContext, nullptr, nullptr);
}

template <>
Object to_ruby<ComparisonContextProxy*>(ComparisonContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ComparisonContextProxy>(x, rb_cComparisonContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Comp_opContext*>(Python3Parser::Comp_opContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Comp_opContext>(x, rb_cComp_opContext, nullptr, nullptr);
}

template <>
Object to_ruby<Comp_opContextProxy*>(Comp_opContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Comp_opContextProxy>(x, rb_cComp_opContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Xor_exprContext*>(Python3Parser::Xor_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Xor_exprContext>(x, rb_cXor_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Xor_exprContextProxy*>(Xor_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Xor_exprContextProxy>(x, rb_cXor_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::And_exprContext*>(Python3Parser::And_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::And_exprContext>(x, rb_cAnd_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<And_exprContextProxy*>(And_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<And_exprContextProxy>(x, rb_cAnd_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Shift_exprContext*>(Python3Parser::Shift_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Shift_exprContext>(x, rb_cShift_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Shift_exprContextProxy*>(Shift_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Shift_exprContextProxy>(x, rb_cShift_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Arith_exprContext*>(Python3Parser::Arith_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Arith_exprContext>(x, rb_cArith_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Arith_exprContextProxy*>(Arith_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Arith_exprContextProxy>(x, rb_cArith_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::TermContext*>(Python3Parser::TermContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::TermContext>(x, rb_cTermContext, nullptr, nullptr);
}

template <>
Object to_ruby<TermContextProxy*>(TermContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<TermContextProxy>(x, rb_cTermContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::FactorContext*>(Python3Parser::FactorContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::FactorContext>(x, rb_cFactorContext, nullptr, nullptr);
}

template <>
Object to_ruby<FactorContextProxy*>(FactorContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<FactorContextProxy>(x, rb_cFactorContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::PowerContext*>(Python3Parser::PowerContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::PowerContext>(x, rb_cPowerContext, nullptr, nullptr);
}

template <>
Object to_ruby<PowerContextProxy*>(PowerContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<PowerContextProxy>(x, rb_cPowerContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Atom_exprContext*>(Python3Parser::Atom_exprContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Atom_exprContext>(x, rb_cAtom_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Atom_exprContextProxy*>(Atom_exprContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Atom_exprContextProxy>(x, rb_cAtom_exprContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::AtomContext*>(Python3Parser::AtomContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::AtomContext>(x, rb_cAtomContext, nullptr, nullptr);
}

template <>
Object to_ruby<AtomContextProxy*>(AtomContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<AtomContextProxy>(x, rb_cAtomContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::TrailerContext*>(Python3Parser::TrailerContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::TrailerContext>(x, rb_cTrailerContext, nullptr, nullptr);
}

template <>
Object to_ruby<TrailerContextProxy*>(TrailerContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<TrailerContextProxy>(x, rb_cTrailerContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Testlist_compContext*>(Python3Parser::Testlist_compContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Testlist_compContext>(x, rb_cTestlist_compContext, nullptr, nullptr);
}

template <>
Object to_ruby<Testlist_compContextProxy*>(Testlist_compContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Testlist_compContextProxy>(x, rb_cTestlist_compContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::DictorsetmakerContext*>(Python3Parser::DictorsetmakerContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::DictorsetmakerContext>(x, rb_cDictorsetmakerContext, nullptr, nullptr);
}

template <>
Object to_ruby<DictorsetmakerContextProxy*>(DictorsetmakerContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<DictorsetmakerContextProxy>(x, rb_cDictorsetmakerContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Comp_forContext*>(Python3Parser::Comp_forContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Comp_forContext>(x, rb_cComp_forContext, nullptr, nullptr);
}

template <>
Object to_ruby<Comp_forContextProxy*>(Comp_forContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Comp_forContextProxy>(x, rb_cComp_forContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::SubscriptlistContext*>(Python3Parser::SubscriptlistContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::SubscriptlistContext>(x, rb_cSubscriptlistContext, nullptr, nullptr);
}

template <>
Object to_ruby<SubscriptlistContextProxy*>(SubscriptlistContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<SubscriptlistContextProxy>(x, rb_cSubscriptlistContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::SubscriptContext*>(Python3Parser::SubscriptContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::SubscriptContext>(x, rb_cSubscriptContext, nullptr, nullptr);
}

template <>
Object to_ruby<SubscriptContextProxy*>(SubscriptContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<SubscriptContextProxy>(x, rb_cSubscriptContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::SliceopContext*>(Python3Parser::SliceopContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::SliceopContext>(x, rb_cSliceopContext, nullptr, nullptr);
}

template <>
Object to_ruby<SliceopContextProxy*>(SliceopContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<SliceopContextProxy>(x, rb_cSliceopContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::ArgumentContext*>(Python3Parser::ArgumentContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::ArgumentContext>(x, rb_cArgumentContext, nullptr, nullptr);
}

template <>
Object to_ruby<ArgumentContextProxy*>(ArgumentContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ArgumentContextProxy>(x, rb_cArgumentContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Comp_iterContext*>(Python3Parser::Comp_iterContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Comp_iterContext>(x, rb_cComp_iterContext, nullptr, nullptr);
}

template <>
Object to_ruby<Comp_iterContextProxy*>(Comp_iterContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Comp_iterContextProxy>(x, rb_cComp_iterContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Comp_ifContext*>(Python3Parser::Comp_ifContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Comp_ifContext>(x, rb_cComp_ifContext, nullptr, nullptr);
}

template <>
Object to_ruby<Comp_ifContextProxy*>(Comp_ifContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Comp_ifContextProxy>(x, rb_cComp_ifContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Encoding_declContext*>(Python3Parser::Encoding_declContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Encoding_declContext>(x, rb_cEncoding_declContext, nullptr, nullptr);
}

template <>
Object to_ruby<Encoding_declContextProxy*>(Encoding_declContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Encoding_declContextProxy>(x, rb_cEncoding_declContext, nullptr, nullptr);
}

template <>
Object to_ruby<Python3Parser::Yield_argContext*>(Python3Parser::Yield_argContext* const &x) {
  if (!x) return Nil;
  return Data_Object<Python3Parser::Yield_argContext>(x, rb_cYield_argContext, nullptr, nullptr);
}

template <>
Object to_ruby<Yield_argContextProxy*>(Yield_argContextProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<Yield_argContextProxy>(x, rb_cYield_argContext, nullptr, nullptr);
}



Object Single_inputContextProxy::simple_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Single_inputContext*)orig) -> simple_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Simple_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Single_inputContextProxy::compound_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Single_inputContext*)orig) -> compound_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Compound_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Single_inputContextProxy::NEWLINE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Single_inputContext*)orig) -> NEWLINE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Simple_stmtContextProxy::small_stmt() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Simple_stmtContext*)orig) -> small_stmt().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(small_stmtAt(i));
    }
  }

  return a;
}

Object Simple_stmtContextProxy::small_stmtAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Simple_stmtContext*)orig) -> small_stmt(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Small_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Simple_stmtContextProxy::NEWLINE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Simple_stmtContext*)orig) -> NEWLINE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Simple_stmtContextProxy::SEMI_COLON() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Simple_stmtContext*)orig) -> SEMI_COLON();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Simple_stmtContextProxy::SEMI_COLONAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Simple_stmtContext*)orig) -> SEMI_COLON(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::if_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> if_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  If_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::while_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> while_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  While_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::for_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> for_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  For_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::try_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> try_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Try_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::with_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> with_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  With_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::funcdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> funcdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  FuncdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::classdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> classdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  ClassdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::decorated() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> decorated();

  if (ctx == nullptr) {
    return Qnil;
  }

  DecoratedContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Compound_stmtContextProxy::async_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Compound_stmtContext*)orig) -> async_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Async_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object File_inputContextProxy::stmt() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::File_inputContext*)orig) -> stmt().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(stmtAt(i));
    }
  }

  return a;
}

Object File_inputContextProxy::stmtAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::File_inputContext*)orig) -> stmt(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  StmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object File_inputContextProxy::EOF() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::File_inputContext*)orig) -> EOF();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object File_inputContextProxy::NEWLINE() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::File_inputContext*)orig) -> NEWLINE();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object File_inputContextProxy::NEWLINEAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::File_inputContext*)orig) -> NEWLINE(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object StmtContextProxy::simple_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::StmtContext*)orig) -> simple_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Simple_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object StmtContextProxy::compound_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::StmtContext*)orig) -> compound_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Compound_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Eval_inputContextProxy::testlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Eval_inputContext*)orig) -> testlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Eval_inputContextProxy::EOF() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Eval_inputContext*)orig) -> EOF();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Eval_inputContextProxy::NEWLINE() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Eval_inputContext*)orig) -> NEWLINE();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Eval_inputContextProxy::NEWLINEAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Eval_inputContext*)orig) -> NEWLINE(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TestlistContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::TestlistContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object TestlistContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TestlistContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TestlistContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TestlistContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TestlistContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TestlistContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DecoratorContextProxy::dotted_name() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratorContext*)orig) -> dotted_name();

  if (ctx == nullptr) {
    return Qnil;
  }

  Dotted_nameContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DecoratorContextProxy::arglist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratorContext*)orig) -> arglist();

  if (ctx == nullptr) {
    return Qnil;
  }

  ArglistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DecoratorContextProxy::AT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DecoratorContext*)orig) -> AT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DecoratorContextProxy::NEWLINE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DecoratorContext*)orig) -> NEWLINE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DecoratorContextProxy::OPEN_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DecoratorContext*)orig) -> OPEN_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DecoratorContextProxy::CLOSE_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DecoratorContext*)orig) -> CLOSE_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Dotted_nameContextProxy::NAME() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Dotted_nameContext*)orig) -> NAME();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Dotted_nameContextProxy::NAMEAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Dotted_nameContext*)orig) -> NAME(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Dotted_nameContextProxy::DOT() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Dotted_nameContext*)orig) -> DOT();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Dotted_nameContextProxy::DOTAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Dotted_nameContext*)orig) -> DOT(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ArglistContextProxy::argument() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ArglistContext*)orig) -> argument().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(argumentAt(i));
    }
  }

  return a;
}

Object ArglistContextProxy::argumentAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ArglistContext*)orig) -> argument(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  ArgumentContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ArglistContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::ArglistContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object ArglistContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ArglistContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DecoratorsContextProxy::decorator() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::DecoratorsContext*)orig) -> decorator().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(decoratorAt(i));
    }
  }

  return a;
}

Object DecoratorsContextProxy::decoratorAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratorsContext*)orig) -> decorator(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  DecoratorContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DecoratedContextProxy::decorators() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratedContext*)orig) -> decorators();

  if (ctx == nullptr) {
    return Qnil;
  }

  DecoratorsContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DecoratedContextProxy::classdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratedContext*)orig) -> classdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  ClassdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DecoratedContextProxy::funcdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratedContext*)orig) -> funcdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  FuncdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DecoratedContextProxy::async_funcdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DecoratedContext*)orig) -> async_funcdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  Async_funcdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::suite() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ClassdefContext*)orig) -> suite();

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::arglist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ClassdefContext*)orig) -> arglist();

  if (ctx == nullptr) {
    return Qnil;
  }

  ArglistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::CLASS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ClassdefContext*)orig) -> CLASS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ClassdefContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ClassdefContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::OPEN_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ClassdefContext*)orig) -> OPEN_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ClassdefContextProxy::CLOSE_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ClassdefContext*)orig) -> CLOSE_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::parameters() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::FuncdefContext*)orig) -> parameters();

  if (ctx == nullptr) {
    return Qnil;
  }

  ParametersContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::suite() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::FuncdefContext*)orig) -> suite();

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::FuncdefContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::DEF() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FuncdefContext*)orig) -> DEF();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FuncdefContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FuncdefContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FuncdefContextProxy::ARROW() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FuncdefContext*)orig) -> ARROW();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Async_funcdefContextProxy::funcdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Async_funcdefContext*)orig) -> funcdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  FuncdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Async_funcdefContextProxy::ASYNC() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Async_funcdefContext*)orig) -> ASYNC();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ParametersContextProxy::typedargslist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ParametersContext*)orig) -> typedargslist();

  if (ctx == nullptr) {
    return Qnil;
  }

  TypedargslistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ParametersContextProxy::OPEN_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ParametersContext*)orig) -> OPEN_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ParametersContextProxy::CLOSE_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ParametersContext*)orig) -> CLOSE_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object SuiteContextProxy::simple_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::SuiteContext*)orig) -> simple_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Simple_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object SuiteContextProxy::stmt() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::SuiteContext*)orig) -> stmt().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(stmtAt(i));
    }
  }

  return a;
}

Object SuiteContextProxy::stmtAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::SuiteContext*)orig) -> stmt(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  StmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object SuiteContextProxy::NEWLINE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::SuiteContext*)orig) -> NEWLINE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object SuiteContextProxy::INDENT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::SuiteContext*)orig) -> INDENT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object SuiteContextProxy::DEDENT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::SuiteContext*)orig) -> DEDENT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TestContextProxy::or_test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::TestContext*)orig) -> or_test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(or_testAt(i));
    }
  }

  return a;
}

Object TestContextProxy::or_testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TestContext*)orig) -> or_test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Or_testContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TestContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TestContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TestContextProxy::lambdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TestContext*)orig) -> lambdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  LambdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TestContextProxy::IF() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TestContext*)orig) -> IF();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TestContextProxy::ELSE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TestContext*)orig) -> ELSE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TypedargslistContextProxy::tfpdef() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::TypedargslistContext*)orig) -> tfpdef().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(tfpdefAt(i));
    }
  }

  return a;
}

Object TypedargslistContextProxy::tfpdefAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TypedargslistContext*)orig) -> tfpdef(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TfpdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TypedargslistContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::TypedargslistContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object TypedargslistContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TypedargslistContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TypedargslistContextProxy::STAR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TypedargslistContext*)orig) -> STAR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TypedargslistContextProxy::POWER() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TypedargslistContext*)orig) -> POWER();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TypedargslistContextProxy::ASSIGN() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TypedargslistContext*)orig) -> ASSIGN();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TypedargslistContextProxy::ASSIGNAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TypedargslistContext*)orig) -> ASSIGN(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TypedargslistContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TypedargslistContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TypedargslistContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TypedargslistContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TfpdefContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TfpdefContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TfpdefContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TfpdefContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TfpdefContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TfpdefContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object VarargslistContextProxy::vfpdef() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::VarargslistContext*)orig) -> vfpdef().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(vfpdefAt(i));
    }
  }

  return a;
}

Object VarargslistContextProxy::vfpdefAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::VarargslistContext*)orig) -> vfpdef(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  VfpdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object VarargslistContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::VarargslistContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object VarargslistContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::VarargslistContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object VarargslistContextProxy::STAR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::VarargslistContext*)orig) -> STAR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object VarargslistContextProxy::POWER() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::VarargslistContext*)orig) -> POWER();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object VarargslistContextProxy::ASSIGN() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::VarargslistContext*)orig) -> ASSIGN();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object VarargslistContextProxy::ASSIGNAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::VarargslistContext*)orig) -> ASSIGN(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object VarargslistContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::VarargslistContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object VarargslistContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::VarargslistContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object VfpdefContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::VfpdefContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::expr_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> expr_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Expr_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::del_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> del_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Del_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::pass_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> pass_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Pass_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::flow_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> flow_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Flow_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::import_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> import_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Import_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::global_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> global_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Global_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::nonlocal_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> nonlocal_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Nonlocal_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Small_stmtContextProxy::assert_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Small_stmtContext*)orig) -> assert_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Assert_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Expr_stmtContextProxy::testlist_star_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Expr_stmtContext*)orig) -> testlist_star_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testlist_star_exprAt(i));
    }
  }

  return a;
}

Object Expr_stmtContextProxy::testlist_star_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Expr_stmtContext*)orig) -> testlist_star_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Testlist_star_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Expr_stmtContextProxy::annassign() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Expr_stmtContext*)orig) -> annassign();

  if (ctx == nullptr) {
    return Qnil;
  }

  AnnassignContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Expr_stmtContextProxy::augassign() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Expr_stmtContext*)orig) -> augassign();

  if (ctx == nullptr) {
    return Qnil;
  }

  AugassignContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Expr_stmtContextProxy::yield_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Expr_stmtContext*)orig) -> yield_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(yield_exprAt(i));
    }
  }

  return a;
}

Object Expr_stmtContextProxy::yield_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Expr_stmtContext*)orig) -> yield_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Yield_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Expr_stmtContextProxy::testlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Expr_stmtContext*)orig) -> testlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Expr_stmtContextProxy::ASSIGN() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Expr_stmtContext*)orig) -> ASSIGN();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Expr_stmtContextProxy::ASSIGNAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Expr_stmtContext*)orig) -> ASSIGN(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Del_stmtContextProxy::exprlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Del_stmtContext*)orig) -> exprlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Del_stmtContextProxy::DEL() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Del_stmtContext*)orig) -> DEL();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Pass_stmtContextProxy::PASS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Pass_stmtContext*)orig) -> PASS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Flow_stmtContextProxy::break_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Flow_stmtContext*)orig) -> break_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Break_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Flow_stmtContextProxy::continue_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Flow_stmtContext*)orig) -> continue_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Continue_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Flow_stmtContextProxy::return_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Flow_stmtContext*)orig) -> return_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Return_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Flow_stmtContextProxy::raise_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Flow_stmtContext*)orig) -> raise_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Raise_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Flow_stmtContextProxy::yield_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Flow_stmtContext*)orig) -> yield_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  Yield_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_stmtContextProxy::import_name() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Import_stmtContext*)orig) -> import_name();

  if (ctx == nullptr) {
    return Qnil;
  }

  Import_nameContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_stmtContextProxy::import_from() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Import_stmtContext*)orig) -> import_from();

  if (ctx == nullptr) {
    return Qnil;
  }

  Import_fromContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Global_stmtContextProxy::GLOBAL() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Global_stmtContext*)orig) -> GLOBAL();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Global_stmtContextProxy::NAME() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Global_stmtContext*)orig) -> NAME();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Global_stmtContextProxy::NAMEAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Global_stmtContext*)orig) -> NAME(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Global_stmtContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Global_stmtContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Global_stmtContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Global_stmtContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Nonlocal_stmtContextProxy::NONLOCAL() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Nonlocal_stmtContext*)orig) -> NONLOCAL();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Nonlocal_stmtContextProxy::NAME() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Nonlocal_stmtContext*)orig) -> NAME();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Nonlocal_stmtContextProxy::NAMEAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Nonlocal_stmtContext*)orig) -> NAME(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Nonlocal_stmtContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Nonlocal_stmtContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Nonlocal_stmtContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Nonlocal_stmtContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Assert_stmtContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Assert_stmtContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object Assert_stmtContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Assert_stmtContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Assert_stmtContextProxy::ASSERT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Assert_stmtContext*)orig) -> ASSERT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Assert_stmtContextProxy::COMMA() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Assert_stmtContext*)orig) -> COMMA();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Testlist_star_exprContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Testlist_star_exprContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object Testlist_star_exprContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Testlist_star_exprContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Testlist_star_exprContextProxy::star_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Testlist_star_exprContext*)orig) -> star_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(star_exprAt(i));
    }
  }

  return a;
}

Object Testlist_star_exprContextProxy::star_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Testlist_star_exprContext*)orig) -> star_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Star_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Testlist_star_exprContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Testlist_star_exprContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Testlist_star_exprContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Testlist_star_exprContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AnnassignContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::AnnassignContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object AnnassignContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::AnnassignContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object AnnassignContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AnnassignContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AnnassignContextProxy::ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AnnassignContext*)orig) -> ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::ADD_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> ADD_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::SUB_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> SUB_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::MULT_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> MULT_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::AT_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> AT_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::DIV_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> DIV_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::MOD_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> MOD_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::AND_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> AND_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::OR_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> OR_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::XOR_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> XOR_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::LEFT_SHIFT_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> LEFT_SHIFT_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::RIGHT_SHIFT_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> RIGHT_SHIFT_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::POWER_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> POWER_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AugassignContextProxy::IDIV_ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AugassignContext*)orig) -> IDIV_ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Yield_exprContextProxy::yield_arg() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Yield_exprContext*)orig) -> yield_arg();

  if (ctx == nullptr) {
    return Qnil;
  }

  Yield_argContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Yield_exprContextProxy::YIELD() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Yield_exprContext*)orig) -> YIELD();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Star_exprContextProxy::expr() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Star_exprContext*)orig) -> expr();

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Star_exprContextProxy::STAR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Star_exprContext*)orig) -> STAR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ExprlistContextProxy::expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ExprlistContext*)orig) -> expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(exprAt(i));
    }
  }

  return a;
}

Object ExprlistContextProxy::exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ExprlistContext*)orig) -> expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ExprlistContextProxy::star_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ExprlistContext*)orig) -> star_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(star_exprAt(i));
    }
  }

  return a;
}

Object ExprlistContextProxy::star_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ExprlistContext*)orig) -> star_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Star_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ExprlistContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::ExprlistContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object ExprlistContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ExprlistContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Break_stmtContextProxy::BREAK() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Break_stmtContext*)orig) -> BREAK();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Continue_stmtContextProxy::CONTINUE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Continue_stmtContext*)orig) -> CONTINUE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Return_stmtContextProxy::testlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Return_stmtContext*)orig) -> testlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Return_stmtContextProxy::RETURN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Return_stmtContext*)orig) -> RETURN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Raise_stmtContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Raise_stmtContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object Raise_stmtContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Raise_stmtContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Raise_stmtContextProxy::RAISE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Raise_stmtContext*)orig) -> RAISE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Raise_stmtContextProxy::FROM() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Raise_stmtContext*)orig) -> FROM();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Yield_stmtContextProxy::yield_expr() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Yield_stmtContext*)orig) -> yield_expr();

  if (ctx == nullptr) {
    return Qnil;
  }

  Yield_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_nameContextProxy::dotted_as_names() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Import_nameContext*)orig) -> dotted_as_names();

  if (ctx == nullptr) {
    return Qnil;
  }

  Dotted_as_namesContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_nameContextProxy::IMPORT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_nameContext*)orig) -> IMPORT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::dotted_name() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Import_fromContext*)orig) -> dotted_name();

  if (ctx == nullptr) {
    return Qnil;
  }

  Dotted_nameContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::import_as_names() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Import_fromContext*)orig) -> import_as_names();

  if (ctx == nullptr) {
    return Qnil;
  }

  Import_as_namesContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::FROM() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> FROM();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::IMPORT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> IMPORT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::STAR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> STAR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::OPEN_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> OPEN_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::CLOSE_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> CLOSE_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::DOT() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Import_fromContext*)orig) -> DOT();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Import_fromContextProxy::DOTAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> DOT(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_fromContextProxy::ELLIPSIS() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Import_fromContext*)orig) -> ELLIPSIS();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Import_fromContextProxy::ELLIPSISAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_fromContext*)orig) -> ELLIPSIS(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Dotted_as_namesContextProxy::dotted_as_name() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Dotted_as_namesContext*)orig) -> dotted_as_name().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(dotted_as_nameAt(i));
    }
  }

  return a;
}

Object Dotted_as_namesContextProxy::dotted_as_nameAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Dotted_as_namesContext*)orig) -> dotted_as_name(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Dotted_as_nameContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Dotted_as_namesContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Dotted_as_namesContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Dotted_as_namesContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Dotted_as_namesContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_as_namesContextProxy::import_as_name() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Import_as_namesContext*)orig) -> import_as_name().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(import_as_nameAt(i));
    }
  }

  return a;
}

Object Import_as_namesContextProxy::import_as_nameAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Import_as_namesContext*)orig) -> import_as_name(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Import_as_nameContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Import_as_namesContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Import_as_namesContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Import_as_namesContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_as_namesContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_as_nameContextProxy::NAME() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Import_as_nameContext*)orig) -> NAME();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Import_as_nameContextProxy::NAMEAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_as_nameContext*)orig) -> NAME(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Import_as_nameContextProxy::AS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Import_as_nameContext*)orig) -> AS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Dotted_as_nameContextProxy::dotted_name() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Dotted_as_nameContext*)orig) -> dotted_name();

  if (ctx == nullptr) {
    return Qnil;
  }

  Dotted_nameContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Dotted_as_nameContextProxy::AS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Dotted_as_nameContext*)orig) -> AS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Dotted_as_nameContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Dotted_as_nameContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object If_stmtContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::If_stmtContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object If_stmtContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::If_stmtContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object If_stmtContextProxy::suite() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::If_stmtContext*)orig) -> suite().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(suiteAt(i));
    }
  }

  return a;
}

Object If_stmtContextProxy::suiteAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::If_stmtContext*)orig) -> suite(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object If_stmtContextProxy::IF() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::If_stmtContext*)orig) -> IF();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object If_stmtContextProxy::COLON() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::If_stmtContext*)orig) -> COLON();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object If_stmtContextProxy::COLONAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::If_stmtContext*)orig) -> COLON(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object If_stmtContextProxy::ELIF() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::If_stmtContext*)orig) -> ELIF();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object If_stmtContextProxy::ELIFAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::If_stmtContext*)orig) -> ELIF(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object If_stmtContextProxy::ELSE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::If_stmtContext*)orig) -> ELSE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object While_stmtContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::While_stmtContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object While_stmtContextProxy::suite() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::While_stmtContext*)orig) -> suite().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(suiteAt(i));
    }
  }

  return a;
}

Object While_stmtContextProxy::suiteAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::While_stmtContext*)orig) -> suite(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object While_stmtContextProxy::WHILE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::While_stmtContext*)orig) -> WHILE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object While_stmtContextProxy::COLON() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::While_stmtContext*)orig) -> COLON();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object While_stmtContextProxy::COLONAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::While_stmtContext*)orig) -> COLON(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object While_stmtContextProxy::ELSE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::While_stmtContext*)orig) -> ELSE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::exprlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::For_stmtContext*)orig) -> exprlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::testlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::For_stmtContext*)orig) -> testlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::suite() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::For_stmtContext*)orig) -> suite().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(suiteAt(i));
    }
  }

  return a;
}

Object For_stmtContextProxy::suiteAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::For_stmtContext*)orig) -> suite(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::FOR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::For_stmtContext*)orig) -> FOR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::IN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::For_stmtContext*)orig) -> IN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::COLON() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::For_stmtContext*)orig) -> COLON();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object For_stmtContextProxy::COLONAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::For_stmtContext*)orig) -> COLON(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object For_stmtContextProxy::ELSE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::For_stmtContext*)orig) -> ELSE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Try_stmtContextProxy::suite() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Try_stmtContext*)orig) -> suite().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(suiteAt(i));
    }
  }

  return a;
}

Object Try_stmtContextProxy::suiteAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Try_stmtContext*)orig) -> suite(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Try_stmtContextProxy::except_clause() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Try_stmtContext*)orig) -> except_clause().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(except_clauseAt(i));
    }
  }

  return a;
}

Object Try_stmtContextProxy::except_clauseAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Try_stmtContext*)orig) -> except_clause(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Except_clauseContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Try_stmtContextProxy::TRY() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Try_stmtContext*)orig) -> TRY();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Try_stmtContextProxy::COLON() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Try_stmtContext*)orig) -> COLON();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Try_stmtContextProxy::COLONAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Try_stmtContext*)orig) -> COLON(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Try_stmtContextProxy::FINALLY() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Try_stmtContext*)orig) -> FINALLY();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Try_stmtContextProxy::ELSE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Try_stmtContext*)orig) -> ELSE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object With_stmtContextProxy::with_item() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::With_stmtContext*)orig) -> with_item().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(with_itemAt(i));
    }
  }

  return a;
}

Object With_stmtContextProxy::with_itemAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::With_stmtContext*)orig) -> with_item(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  With_itemContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object With_stmtContextProxy::suite() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::With_stmtContext*)orig) -> suite();

  if (ctx == nullptr) {
    return Qnil;
  }

  SuiteContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object With_stmtContextProxy::WITH() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::With_stmtContext*)orig) -> WITH();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object With_stmtContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::With_stmtContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object With_stmtContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::With_stmtContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object With_stmtContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::With_stmtContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Async_stmtContextProxy::funcdef() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Async_stmtContext*)orig) -> funcdef();

  if (ctx == nullptr) {
    return Qnil;
  }

  FuncdefContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Async_stmtContextProxy::with_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Async_stmtContext*)orig) -> with_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  With_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Async_stmtContextProxy::for_stmt() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Async_stmtContext*)orig) -> for_stmt();

  if (ctx == nullptr) {
    return Qnil;
  }

  For_stmtContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Async_stmtContextProxy::ASYNC() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Async_stmtContext*)orig) -> ASYNC();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Except_clauseContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Except_clauseContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Except_clauseContextProxy::EXCEPT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Except_clauseContext*)orig) -> EXCEPT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Except_clauseContextProxy::AS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Except_clauseContext*)orig) -> AS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Except_clauseContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Except_clauseContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object With_itemContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::With_itemContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object With_itemContextProxy::expr() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::With_itemContext*)orig) -> expr();

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object With_itemContextProxy::AS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::With_itemContext*)orig) -> AS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ExprContextProxy::xor_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ExprContext*)orig) -> xor_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(xor_exprAt(i));
    }
  }

  return a;
}

Object ExprContextProxy::xor_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ExprContext*)orig) -> xor_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Xor_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ExprContextProxy::OR_OP() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::ExprContext*)orig) -> OR_OP();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object ExprContextProxy::OR_OPAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ExprContext*)orig) -> OR_OP(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Or_testContextProxy::and_test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Or_testContext*)orig) -> and_test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(and_testAt(i));
    }
  }

  return a;
}

Object Or_testContextProxy::and_testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Or_testContext*)orig) -> and_test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  And_testContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Or_testContextProxy::OR() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Or_testContext*)orig) -> OR();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Or_testContextProxy::ORAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Or_testContext*)orig) -> OR(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object LambdefContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::LambdefContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object LambdefContextProxy::varargslist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::LambdefContext*)orig) -> varargslist();

  if (ctx == nullptr) {
    return Qnil;
  }

  VarargslistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object LambdefContextProxy::LAMBDA() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::LambdefContext*)orig) -> LAMBDA();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object LambdefContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::LambdefContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Test_nocondContextProxy::or_test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Test_nocondContext*)orig) -> or_test();

  if (ctx == nullptr) {
    return Qnil;
  }

  Or_testContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Test_nocondContextProxy::lambdef_nocond() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Test_nocondContext*)orig) -> lambdef_nocond();

  if (ctx == nullptr) {
    return Qnil;
  }

  Lambdef_nocondContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Lambdef_nocondContextProxy::test_nocond() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Lambdef_nocondContext*)orig) -> test_nocond();

  if (ctx == nullptr) {
    return Qnil;
  }

  Test_nocondContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Lambdef_nocondContextProxy::varargslist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Lambdef_nocondContext*)orig) -> varargslist();

  if (ctx == nullptr) {
    return Qnil;
  }

  VarargslistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Lambdef_nocondContextProxy::LAMBDA() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Lambdef_nocondContext*)orig) -> LAMBDA();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Lambdef_nocondContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Lambdef_nocondContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object And_testContextProxy::not_test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::And_testContext*)orig) -> not_test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(not_testAt(i));
    }
  }

  return a;
}

Object And_testContextProxy::not_testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::And_testContext*)orig) -> not_test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Not_testContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object And_testContextProxy::AND() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::And_testContext*)orig) -> AND();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object And_testContextProxy::ANDAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::And_testContext*)orig) -> AND(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Not_testContextProxy::not_test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Not_testContext*)orig) -> not_test();

  if (ctx == nullptr) {
    return Qnil;
  }

  Not_testContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Not_testContextProxy::comparison() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Not_testContext*)orig) -> comparison();

  if (ctx == nullptr) {
    return Qnil;
  }

  ComparisonContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Not_testContextProxy::NOT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Not_testContext*)orig) -> NOT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ComparisonContextProxy::expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ComparisonContext*)orig) -> expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(exprAt(i));
    }
  }

  return a;
}

Object ComparisonContextProxy::exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ComparisonContext*)orig) -> expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ComparisonContextProxy::comp_op() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ComparisonContext*)orig) -> comp_op().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(comp_opAt(i));
    }
  }

  return a;
}

Object ComparisonContextProxy::comp_opAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ComparisonContext*)orig) -> comp_op(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_opContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::LESS_THAN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> LESS_THAN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::GREATER_THAN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> GREATER_THAN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::EQUALS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> EQUALS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::GT_EQ() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> GT_EQ();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::LT_EQ() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> LT_EQ();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::NOT_EQ_1() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> NOT_EQ_1();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::NOT_EQ_2() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> NOT_EQ_2();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::IN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> IN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::NOT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> NOT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_opContextProxy::IS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_opContext*)orig) -> IS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Xor_exprContextProxy::and_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Xor_exprContext*)orig) -> and_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(and_exprAt(i));
    }
  }

  return a;
}

Object Xor_exprContextProxy::and_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Xor_exprContext*)orig) -> and_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  And_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Xor_exprContextProxy::XOR() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Xor_exprContext*)orig) -> XOR();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Xor_exprContextProxy::XORAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Xor_exprContext*)orig) -> XOR(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object And_exprContextProxy::shift_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::And_exprContext*)orig) -> shift_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(shift_exprAt(i));
    }
  }

  return a;
}

Object And_exprContextProxy::shift_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::And_exprContext*)orig) -> shift_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Shift_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object And_exprContextProxy::AND_OP() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::And_exprContext*)orig) -> AND_OP();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object And_exprContextProxy::AND_OPAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::And_exprContext*)orig) -> AND_OP(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Shift_exprContextProxy::arith_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Shift_exprContext*)orig) -> arith_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(arith_exprAt(i));
    }
  }

  return a;
}

Object Shift_exprContextProxy::arith_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Shift_exprContext*)orig) -> arith_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Arith_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Shift_exprContextProxy::LEFT_SHIFT() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Shift_exprContext*)orig) -> LEFT_SHIFT();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Shift_exprContextProxy::LEFT_SHIFTAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Shift_exprContext*)orig) -> LEFT_SHIFT(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Shift_exprContextProxy::RIGHT_SHIFT() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Shift_exprContext*)orig) -> RIGHT_SHIFT();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Shift_exprContextProxy::RIGHT_SHIFTAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Shift_exprContext*)orig) -> RIGHT_SHIFT(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Arith_exprContextProxy::term() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Arith_exprContext*)orig) -> term().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(termAt(i));
    }
  }

  return a;
}

Object Arith_exprContextProxy::termAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Arith_exprContext*)orig) -> term(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TermContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Arith_exprContextProxy::ADD() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Arith_exprContext*)orig) -> ADD();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Arith_exprContextProxy::ADDAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Arith_exprContext*)orig) -> ADD(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Arith_exprContextProxy::MINUS() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Arith_exprContext*)orig) -> MINUS();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Arith_exprContextProxy::MINUSAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Arith_exprContext*)orig) -> MINUS(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TermContextProxy::factor() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::TermContext*)orig) -> factor().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(factorAt(i));
    }
  }

  return a;
}

Object TermContextProxy::factorAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TermContext*)orig) -> factor(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  FactorContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TermContextProxy::STAR() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TermContext*)orig) -> STAR();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TermContextProxy::STARAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TermContext*)orig) -> STAR(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TermContextProxy::AT() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TermContext*)orig) -> AT();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TermContextProxy::ATAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TermContext*)orig) -> AT(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TermContextProxy::DIV() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TermContext*)orig) -> DIV();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TermContextProxy::DIVAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TermContext*)orig) -> DIV(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TermContextProxy::MOD() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TermContext*)orig) -> MOD();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TermContextProxy::MODAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TermContext*)orig) -> MOD(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TermContextProxy::IDIV() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::TermContext*)orig) -> IDIV();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object TermContextProxy::IDIVAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TermContext*)orig) -> IDIV(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FactorContextProxy::factor() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::FactorContext*)orig) -> factor();

  if (ctx == nullptr) {
    return Qnil;
  }

  FactorContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object FactorContextProxy::power() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::FactorContext*)orig) -> power();

  if (ctx == nullptr) {
    return Qnil;
  }

  PowerContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object FactorContextProxy::ADD() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FactorContext*)orig) -> ADD();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FactorContextProxy::MINUS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FactorContext*)orig) -> MINUS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object FactorContextProxy::NOT_OP() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::FactorContext*)orig) -> NOT_OP();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object PowerContextProxy::atom_expr() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::PowerContext*)orig) -> atom_expr();

  if (ctx == nullptr) {
    return Qnil;
  }

  Atom_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object PowerContextProxy::factor() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::PowerContext*)orig) -> factor();

  if (ctx == nullptr) {
    return Qnil;
  }

  FactorContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object PowerContextProxy::POWER() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::PowerContext*)orig) -> POWER();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Atom_exprContextProxy::atom() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Atom_exprContext*)orig) -> atom();

  if (ctx == nullptr) {
    return Qnil;
  }

  AtomContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Atom_exprContextProxy::trailer() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Atom_exprContext*)orig) -> trailer().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(trailerAt(i));
    }
  }

  return a;
}

Object Atom_exprContextProxy::trailerAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Atom_exprContext*)orig) -> trailer(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TrailerContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Atom_exprContextProxy::AWAIT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Atom_exprContext*)orig) -> AWAIT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::yield_expr() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::AtomContext*)orig) -> yield_expr();

  if (ctx == nullptr) {
    return Qnil;
  }

  Yield_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object AtomContextProxy::testlist_comp() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::AtomContext*)orig) -> testlist_comp();

  if (ctx == nullptr) {
    return Qnil;
  }

  Testlist_compContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object AtomContextProxy::dictorsetmaker() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::AtomContext*)orig) -> dictorsetmaker();

  if (ctx == nullptr) {
    return Qnil;
  }

  DictorsetmakerContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object AtomContextProxy::OPEN_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> OPEN_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::CLOSE_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> CLOSE_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::OPEN_BRACK() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> OPEN_BRACK();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::CLOSE_BRACK() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> CLOSE_BRACK();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::OPEN_BRACE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> OPEN_BRACE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::CLOSE_BRACE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> CLOSE_BRACE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::NUMBER() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> NUMBER();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::ELLIPSIS() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> ELLIPSIS();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::NONE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> NONE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::TRUE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> TRUE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::FALSE() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> FALSE();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object AtomContextProxy::STRING() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::AtomContext*)orig) -> STRING();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object AtomContextProxy::STRINGAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::AtomContext*)orig) -> STRING(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TrailerContextProxy::arglist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TrailerContext*)orig) -> arglist();

  if (ctx == nullptr) {
    return Qnil;
  }

  ArglistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TrailerContextProxy::subscriptlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::TrailerContext*)orig) -> subscriptlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  SubscriptlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object TrailerContextProxy::OPEN_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TrailerContext*)orig) -> OPEN_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TrailerContextProxy::CLOSE_PAREN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TrailerContext*)orig) -> CLOSE_PAREN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TrailerContextProxy::OPEN_BRACK() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TrailerContext*)orig) -> OPEN_BRACK();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TrailerContextProxy::CLOSE_BRACK() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TrailerContext*)orig) -> CLOSE_BRACK();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TrailerContextProxy::DOT() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TrailerContext*)orig) -> DOT();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object TrailerContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::TrailerContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Testlist_compContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Testlist_compContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object Testlist_compContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Testlist_compContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Testlist_compContextProxy::star_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::Testlist_compContext*)orig) -> star_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(star_exprAt(i));
    }
  }

  return a;
}

Object Testlist_compContextProxy::star_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Testlist_compContext*)orig) -> star_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Star_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Testlist_compContextProxy::comp_for() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Testlist_compContext*)orig) -> comp_for();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_forContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Testlist_compContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::Testlist_compContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object Testlist_compContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Testlist_compContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::DictorsetmakerContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object DictorsetmakerContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DictorsetmakerContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::DictorsetmakerContext*)orig) -> expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(exprAt(i));
    }
  }

  return a;
}

Object DictorsetmakerContextProxy::exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DictorsetmakerContext*)orig) -> expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::comp_for() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DictorsetmakerContext*)orig) -> comp_for();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_forContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::star_expr() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::DictorsetmakerContext*)orig) -> star_expr().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(star_exprAt(i));
    }
  }

  return a;
}

Object DictorsetmakerContextProxy::star_exprAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::DictorsetmakerContext*)orig) -> star_expr(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  Star_exprContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::COLON() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::DictorsetmakerContext*)orig) -> COLON();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object DictorsetmakerContextProxy::COLONAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DictorsetmakerContext*)orig) -> COLON(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::POWER() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::DictorsetmakerContext*)orig) -> POWER();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object DictorsetmakerContextProxy::POWERAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DictorsetmakerContext*)orig) -> POWER(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object DictorsetmakerContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::DictorsetmakerContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object DictorsetmakerContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::DictorsetmakerContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_forContextProxy::exprlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_forContext*)orig) -> exprlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  ExprlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_forContextProxy::or_test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_forContext*)orig) -> or_test();

  if (ctx == nullptr) {
    return Qnil;
  }

  Or_testContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_forContextProxy::comp_iter() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_forContext*)orig) -> comp_iter();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_iterContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_forContextProxy::FOR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_forContext*)orig) -> FOR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_forContextProxy::IN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_forContext*)orig) -> IN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_forContextProxy::ASYNC() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_forContext*)orig) -> ASYNC();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object SubscriptlistContextProxy::subscript() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::SubscriptlistContext*)orig) -> subscript().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(subscriptAt(i));
    }
  }

  return a;
}

Object SubscriptlistContextProxy::subscriptAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::SubscriptlistContext*)orig) -> subscript(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  SubscriptContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object SubscriptlistContextProxy::COMMA() {
  Array a;

  if (orig == nullptr) {
    return a;
  }

  auto vec = ((Python3Parser::SubscriptlistContext*)orig) -> COMMA();

  for (auto it = vec.begin(); it != vec.end(); it ++) {
    TerminalNodeProxy proxy(*it);
    a.push(proxy);
  }

  return a;
}

Object SubscriptlistContextProxy::COMMAAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::SubscriptlistContext*)orig) -> COMMA(i);
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object SubscriptContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::SubscriptContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object SubscriptContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::SubscriptContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object SubscriptContextProxy::sliceop() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::SubscriptContext*)orig) -> sliceop();

  if (ctx == nullptr) {
    return Qnil;
  }

  SliceopContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object SubscriptContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::SubscriptContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object SliceopContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::SliceopContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object SliceopContextProxy::COLON() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::SliceopContext*)orig) -> COLON();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ArgumentContextProxy::test() {
  Array a;

  if (orig != nullptr) {
    size_t count = ((Python3Parser::ArgumentContext*)orig) -> test().size();

    for (size_t i = 0; i < count; i ++) {
      a.push(testAt(i));
    }
  }

  return a;
}

Object ArgumentContextProxy::testAt(size_t i) {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ArgumentContext*)orig) -> test(i);

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ArgumentContextProxy::comp_for() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::ArgumentContext*)orig) -> comp_for();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_forContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object ArgumentContextProxy::ASSIGN() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ArgumentContext*)orig) -> ASSIGN();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ArgumentContextProxy::POWER() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ArgumentContext*)orig) -> POWER();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object ArgumentContextProxy::STAR() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::ArgumentContext*)orig) -> STAR();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Comp_iterContextProxy::comp_for() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_iterContext*)orig) -> comp_for();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_forContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_iterContextProxy::comp_if() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_iterContext*)orig) -> comp_if();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_ifContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_ifContextProxy::test_nocond() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_ifContext*)orig) -> test_nocond();

  if (ctx == nullptr) {
    return Qnil;
  }

  Test_nocondContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_ifContextProxy::comp_iter() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Comp_ifContext*)orig) -> comp_iter();

  if (ctx == nullptr) {
    return Qnil;
  }

  Comp_iterContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Comp_ifContextProxy::IF() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Comp_ifContext*)orig) -> IF();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Encoding_declContextProxy::NAME() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Encoding_declContext*)orig) -> NAME();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}

Object Yield_argContextProxy::test() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Yield_argContext*)orig) -> test();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Yield_argContextProxy::testlist() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto ctx = ((Python3Parser::Yield_argContext*)orig) -> testlist();

  if (ctx == nullptr) {
    return Qnil;
  }

  TestlistContextProxy proxy(ctx);
  return to_ruby(proxy);
}

Object Yield_argContextProxy::FROM() {
  if (orig == nullptr) {
    return Qnil;
  }

  auto token = ((Python3Parser::Yield_argContext*)orig) -> FROM();
  TerminalNodeProxy proxy(token);
  return to_ruby(proxy);
}


class VisitorProxy : public Python3ParserBaseVisitor, public Director {
public:
  VisitorProxy(Object self) : Director(self) { }

  Object ruby_visit(ContextProxy* proxy) {
    visit(proxy -> getOriginal());
    return Nil;
  }

  Object ruby_visitChildren(ContextProxy* proxy) {
    visitChildren(proxy -> getOriginal());
    return Nil;
  }

  virtual antlrcpp::Any visitSingle_input(Python3Parser::Single_inputContext *ctx) override {
    Single_inputContextProxy proxy(ctx);
    return getSelf().call("visit_single_input", &proxy);
  }

  virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
    File_inputContextProxy proxy(ctx);
    return getSelf().call("visit_file_input", &proxy);
  }

  virtual antlrcpp::Any visitEval_input(Python3Parser::Eval_inputContext *ctx) override {
    Eval_inputContextProxy proxy(ctx);
    return getSelf().call("visit_eval_input", &proxy);
  }

  virtual antlrcpp::Any visitDecorator(Python3Parser::DecoratorContext *ctx) override {
    DecoratorContextProxy proxy(ctx);
    return getSelf().call("visit_decorator", &proxy);
  }

  virtual antlrcpp::Any visitDecorators(Python3Parser::DecoratorsContext *ctx) override {
    DecoratorsContextProxy proxy(ctx);
    return getSelf().call("visit_decorators", &proxy);
  }

  virtual antlrcpp::Any visitDecorated(Python3Parser::DecoratedContext *ctx) override {
    DecoratedContextProxy proxy(ctx);
    return getSelf().call("visit_decorated", &proxy);
  }

  virtual antlrcpp::Any visitAsync_funcdef(Python3Parser::Async_funcdefContext *ctx) override {
    Async_funcdefContextProxy proxy(ctx);
    return getSelf().call("visit_async_funcdef", &proxy);
  }

  virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
    FuncdefContextProxy proxy(ctx);
    return getSelf().call("visit_funcdef", &proxy);
  }

  virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
    ParametersContextProxy proxy(ctx);
    return getSelf().call("visit_parameters", &proxy);
  }

  virtual antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
    TypedargslistContextProxy proxy(ctx);
    return getSelf().call("visit_typedargslist", &proxy);
  }

  virtual antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
    TfpdefContextProxy proxy(ctx);
    return getSelf().call("visit_tfpdef", &proxy);
  }

  virtual antlrcpp::Any visitVarargslist(Python3Parser::VarargslistContext *ctx) override {
    VarargslistContextProxy proxy(ctx);
    return getSelf().call("visit_varargslist", &proxy);
  }

  virtual antlrcpp::Any visitVfpdef(Python3Parser::VfpdefContext *ctx) override {
    VfpdefContextProxy proxy(ctx);
    return getSelf().call("visit_vfpdef", &proxy);
  }

  virtual antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
    StmtContextProxy proxy(ctx);
    return getSelf().call("visit_stmt", &proxy);
  }

  virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
    Simple_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_simple_stmt", &proxy);
  }

  virtual antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
    Small_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_small_stmt", &proxy);
  }

  virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
    Expr_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_expr_stmt", &proxy);
  }

  virtual antlrcpp::Any visitAnnassign(Python3Parser::AnnassignContext *ctx) override {
    AnnassignContextProxy proxy(ctx);
    return getSelf().call("visit_annassign", &proxy);
  }

  virtual antlrcpp::Any visitTestlist_star_expr(Python3Parser::Testlist_star_exprContext *ctx) override {
    Testlist_star_exprContextProxy proxy(ctx);
    return getSelf().call("visit_testlist_star_expr", &proxy);
  }

  virtual antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
    AugassignContextProxy proxy(ctx);
    return getSelf().call("visit_augassign", &proxy);
  }

  virtual antlrcpp::Any visitDel_stmt(Python3Parser::Del_stmtContext *ctx) override {
    Del_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_del_stmt", &proxy);
  }

  virtual antlrcpp::Any visitPass_stmt(Python3Parser::Pass_stmtContext *ctx) override {
    Pass_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_pass_stmt", &proxy);
  }

  virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
    Flow_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_flow_stmt", &proxy);
  }

  virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
    Break_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_break_stmt", &proxy);
  }

  virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
    Continue_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_continue_stmt", &proxy);
  }

  virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    Return_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_return_stmt", &proxy);
  }

  virtual antlrcpp::Any visitYield_stmt(Python3Parser::Yield_stmtContext *ctx) override {
    Yield_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_yield_stmt", &proxy);
  }

  virtual antlrcpp::Any visitRaise_stmt(Python3Parser::Raise_stmtContext *ctx) override {
    Raise_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_raise_stmt", &proxy);
  }

  virtual antlrcpp::Any visitImport_stmt(Python3Parser::Import_stmtContext *ctx) override {
    Import_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_import_stmt", &proxy);
  }

  virtual antlrcpp::Any visitImport_name(Python3Parser::Import_nameContext *ctx) override {
    Import_nameContextProxy proxy(ctx);
    return getSelf().call("visit_import_name", &proxy);
  }

  virtual antlrcpp::Any visitImport_from(Python3Parser::Import_fromContext *ctx) override {
    Import_fromContextProxy proxy(ctx);
    return getSelf().call("visit_import_from", &proxy);
  }

  virtual antlrcpp::Any visitImport_as_name(Python3Parser::Import_as_nameContext *ctx) override {
    Import_as_nameContextProxy proxy(ctx);
    return getSelf().call("visit_import_as_name", &proxy);
  }

  virtual antlrcpp::Any visitDotted_as_name(Python3Parser::Dotted_as_nameContext *ctx) override {
    Dotted_as_nameContextProxy proxy(ctx);
    return getSelf().call("visit_dotted_as_name", &proxy);
  }

  virtual antlrcpp::Any visitImport_as_names(Python3Parser::Import_as_namesContext *ctx) override {
    Import_as_namesContextProxy proxy(ctx);
    return getSelf().call("visit_import_as_names", &proxy);
  }

  virtual antlrcpp::Any visitDotted_as_names(Python3Parser::Dotted_as_namesContext *ctx) override {
    Dotted_as_namesContextProxy proxy(ctx);
    return getSelf().call("visit_dotted_as_names", &proxy);
  }

  virtual antlrcpp::Any visitDotted_name(Python3Parser::Dotted_nameContext *ctx) override {
    Dotted_nameContextProxy proxy(ctx);
    return getSelf().call("visit_dotted_name", &proxy);
  }

  virtual antlrcpp::Any visitGlobal_stmt(Python3Parser::Global_stmtContext *ctx) override {
    Global_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_global_stmt", &proxy);
  }

  virtual antlrcpp::Any visitNonlocal_stmt(Python3Parser::Nonlocal_stmtContext *ctx) override {
    Nonlocal_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_nonlocal_stmt", &proxy);
  }

  virtual antlrcpp::Any visitAssert_stmt(Python3Parser::Assert_stmtContext *ctx) override {
    Assert_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_assert_stmt", &proxy);
  }

  virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
    Compound_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_compound_stmt", &proxy);
  }

  virtual antlrcpp::Any visitAsync_stmt(Python3Parser::Async_stmtContext *ctx) override {
    Async_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_async_stmt", &proxy);
  }

  virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
    If_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_if_stmt", &proxy);
  }

  virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
    While_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_while_stmt", &proxy);
  }

  virtual antlrcpp::Any visitFor_stmt(Python3Parser::For_stmtContext *ctx) override {
    For_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_for_stmt", &proxy);
  }

  virtual antlrcpp::Any visitTry_stmt(Python3Parser::Try_stmtContext *ctx) override {
    Try_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_try_stmt", &proxy);
  }

  virtual antlrcpp::Any visitWith_stmt(Python3Parser::With_stmtContext *ctx) override {
    With_stmtContextProxy proxy(ctx);
    return getSelf().call("visit_with_stmt", &proxy);
  }

  virtual antlrcpp::Any visitWith_item(Python3Parser::With_itemContext *ctx) override {
    With_itemContextProxy proxy(ctx);
    return getSelf().call("visit_with_item", &proxy);
  }

  virtual antlrcpp::Any visitExcept_clause(Python3Parser::Except_clauseContext *ctx) override {
    Except_clauseContextProxy proxy(ctx);
    return getSelf().call("visit_except_clause", &proxy);
  }

  virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
    SuiteContextProxy proxy(ctx);
    return getSelf().call("visit_suite", &proxy);
  }

  virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
    TestContextProxy proxy(ctx);
    return getSelf().call("visit_test", &proxy);
  }

  virtual antlrcpp::Any visitTest_nocond(Python3Parser::Test_nocondContext *ctx) override {
    Test_nocondContextProxy proxy(ctx);
    return getSelf().call("visit_test_nocond", &proxy);
  }

  virtual antlrcpp::Any visitLambdef(Python3Parser::LambdefContext *ctx) override {
    LambdefContextProxy proxy(ctx);
    return getSelf().call("visit_lambdef", &proxy);
  }

  virtual antlrcpp::Any visitLambdef_nocond(Python3Parser::Lambdef_nocondContext *ctx) override {
    Lambdef_nocondContextProxy proxy(ctx);
    return getSelf().call("visit_lambdef_nocond", &proxy);
  }

  virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
    Or_testContextProxy proxy(ctx);
    return getSelf().call("visit_or_test", &proxy);
  }

  virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
    And_testContextProxy proxy(ctx);
    return getSelf().call("visit_and_test", &proxy);
  }

  virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
    Not_testContextProxy proxy(ctx);
    return getSelf().call("visit_not_test", &proxy);
  }

  virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
    ComparisonContextProxy proxy(ctx);
    return getSelf().call("visit_comparison", &proxy);
  }

  virtual antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
    Comp_opContextProxy proxy(ctx);
    return getSelf().call("visit_comp_op", &proxy);
  }

  virtual antlrcpp::Any visitStar_expr(Python3Parser::Star_exprContext *ctx) override {
    Star_exprContextProxy proxy(ctx);
    return getSelf().call("visit_star_expr", &proxy);
  }

  virtual antlrcpp::Any visitExpr(Python3Parser::ExprContext *ctx) override {
    ExprContextProxy proxy(ctx);
    return getSelf().call("visit_expr", &proxy);
  }

  virtual antlrcpp::Any visitXor_expr(Python3Parser::Xor_exprContext *ctx) override {
    Xor_exprContextProxy proxy(ctx);
    return getSelf().call("visit_xor_expr", &proxy);
  }

  virtual antlrcpp::Any visitAnd_expr(Python3Parser::And_exprContext *ctx) override {
    And_exprContextProxy proxy(ctx);
    return getSelf().call("visit_and_expr", &proxy);
  }

  virtual antlrcpp::Any visitShift_expr(Python3Parser::Shift_exprContext *ctx) override {
    Shift_exprContextProxy proxy(ctx);
    return getSelf().call("visit_shift_expr", &proxy);
  }

  virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
    Arith_exprContextProxy proxy(ctx);
    return getSelf().call("visit_arith_expr", &proxy);
  }

  virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
    TermContextProxy proxy(ctx);
    return getSelf().call("visit_term", &proxy);
  }

  virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
    FactorContextProxy proxy(ctx);
    return getSelf().call("visit_factor", &proxy);
  }

  virtual antlrcpp::Any visitPower(Python3Parser::PowerContext *ctx) override {
    PowerContextProxy proxy(ctx);
    return getSelf().call("visit_power", &proxy);
  }

  virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
    Atom_exprContextProxy proxy(ctx);
    return getSelf().call("visit_atom_expr", &proxy);
  }

  virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
    AtomContextProxy proxy(ctx);
    return getSelf().call("visit_atom", &proxy);
  }

  virtual antlrcpp::Any visitTestlist_comp(Python3Parser::Testlist_compContext *ctx) override {
    Testlist_compContextProxy proxy(ctx);
    return getSelf().call("visit_testlist_comp", &proxy);
  }

  virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
    TrailerContextProxy proxy(ctx);
    return getSelf().call("visit_trailer", &proxy);
  }

  virtual antlrcpp::Any visitSubscriptlist(Python3Parser::SubscriptlistContext *ctx) override {
    SubscriptlistContextProxy proxy(ctx);
    return getSelf().call("visit_subscriptlist", &proxy);
  }

  virtual antlrcpp::Any visitSubscript(Python3Parser::SubscriptContext *ctx) override {
    SubscriptContextProxy proxy(ctx);
    return getSelf().call("visit_subscript", &proxy);
  }

  virtual antlrcpp::Any visitSliceop(Python3Parser::SliceopContext *ctx) override {
    SliceopContextProxy proxy(ctx);
    return getSelf().call("visit_sliceop", &proxy);
  }

  virtual antlrcpp::Any visitExprlist(Python3Parser::ExprlistContext *ctx) override {
    ExprlistContextProxy proxy(ctx);
    return getSelf().call("visit_exprlist", &proxy);
  }

  virtual antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
    TestlistContextProxy proxy(ctx);
    return getSelf().call("visit_testlist", &proxy);
  }

  virtual antlrcpp::Any visitDictorsetmaker(Python3Parser::DictorsetmakerContext *ctx) override {
    DictorsetmakerContextProxy proxy(ctx);
    return getSelf().call("visit_dictorsetmaker", &proxy);
  }

  virtual antlrcpp::Any visitClassdef(Python3Parser::ClassdefContext *ctx) override {
    ClassdefContextProxy proxy(ctx);
    return getSelf().call("visit_classdef", &proxy);
  }

  virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
    ArglistContextProxy proxy(ctx);
    return getSelf().call("visit_arglist", &proxy);
  }

  virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
    ArgumentContextProxy proxy(ctx);
    return getSelf().call("visit_argument", &proxy);
  }

  virtual antlrcpp::Any visitComp_iter(Python3Parser::Comp_iterContext *ctx) override {
    Comp_iterContextProxy proxy(ctx);
    return getSelf().call("visit_comp_iter", &proxy);
  }

  virtual antlrcpp::Any visitComp_for(Python3Parser::Comp_forContext *ctx) override {
    Comp_forContextProxy proxy(ctx);
    return getSelf().call("visit_comp_for", &proxy);
  }

  virtual antlrcpp::Any visitComp_if(Python3Parser::Comp_ifContext *ctx) override {
    Comp_ifContextProxy proxy(ctx);
    return getSelf().call("visit_comp_if", &proxy);
  }

  virtual antlrcpp::Any visitEncoding_decl(Python3Parser::Encoding_declContext *ctx) override {
    Encoding_declContextProxy proxy(ctx);
    return getSelf().call("visit_encoding_decl", &proxy);
  }

  virtual antlrcpp::Any visitYield_expr(Python3Parser::Yield_exprContext *ctx) override {
    Yield_exprContextProxy proxy(ctx);
    return getSelf().call("visit_yield_expr", &proxy);
  }

  virtual antlrcpp::Any visitYield_arg(Python3Parser::Yield_argContext *ctx) override {
    Yield_argContextProxy proxy(ctx);
    return getSelf().call("visit_yield_arg", &proxy);
  }

};


class ParserProxy {
public:
  static ParserProxy* parse(string code) {
    auto input = new ANTLRInputStream(code);
    return parseStream(input);
  }

  static ParserProxy* parseFile(string file) {
    ifstream stream;
    stream.open(file);

    auto input = new ANTLRInputStream(stream);
    auto parser = parseStream(input);

    stream.close();

    return parser;
  }

  VALUE visit(VisitorProxy* visitor) {
    visitor -> visit(this -> parser -> file_input());

    // reset for the next visit call
    this -> lexer -> reset();
    this -> parser -> reset();

    return Qnil;
  }

  ~ParserProxy() {
    delete this -> parser;
    delete this -> tokens;
    delete this -> lexer;
    delete this -> input;
  }

private:
  static ParserProxy* parseStream(ANTLRInputStream* input) {
    ParserProxy* parser = new ParserProxy();

    parser -> input = input;
    parser -> lexer = new Python3Lexer(parser -> input);
    parser -> tokens = new CommonTokenStream(parser -> lexer);
    parser -> parser = new Python3Parser(parser -> tokens);

    return parser;
  }

  ParserProxy() {};

  ANTLRInputStream* input;
  Python3Lexer* lexer;
  CommonTokenStream* tokens;
  Python3Parser* parser;
};

template <>
Object to_ruby<ParserProxy*>(ParserProxy* const &x) {
  if (!x) return Nil;
  return Data_Object<ParserProxy>(x, rb_cParser, nullptr, nullptr);
}


Object ContextProxy::wrapParseTree(tree::ParseTree* node) {
  if (antlrcpp::is<Python3Parser::Single_inputContext*>(node)) {
    Single_inputContextProxy proxy((Python3Parser::Single_inputContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Simple_stmtContext*>(node)) {
    Simple_stmtContextProxy proxy((Python3Parser::Simple_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Compound_stmtContext*>(node)) {
    Compound_stmtContextProxy proxy((Python3Parser::Compound_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::File_inputContext*>(node)) {
    File_inputContextProxy proxy((Python3Parser::File_inputContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::StmtContext*>(node)) {
    StmtContextProxy proxy((Python3Parser::StmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Eval_inputContext*>(node)) {
    Eval_inputContextProxy proxy((Python3Parser::Eval_inputContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::TestlistContext*>(node)) {
    TestlistContextProxy proxy((Python3Parser::TestlistContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::DecoratorContext*>(node)) {
    DecoratorContextProxy proxy((Python3Parser::DecoratorContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Dotted_nameContext*>(node)) {
    Dotted_nameContextProxy proxy((Python3Parser::Dotted_nameContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ArglistContext*>(node)) {
    ArglistContextProxy proxy((Python3Parser::ArglistContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::DecoratorsContext*>(node)) {
    DecoratorsContextProxy proxy((Python3Parser::DecoratorsContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::DecoratedContext*>(node)) {
    DecoratedContextProxy proxy((Python3Parser::DecoratedContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ClassdefContext*>(node)) {
    ClassdefContextProxy proxy((Python3Parser::ClassdefContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::FuncdefContext*>(node)) {
    FuncdefContextProxy proxy((Python3Parser::FuncdefContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Async_funcdefContext*>(node)) {
    Async_funcdefContextProxy proxy((Python3Parser::Async_funcdefContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ParametersContext*>(node)) {
    ParametersContextProxy proxy((Python3Parser::ParametersContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::SuiteContext*>(node)) {
    SuiteContextProxy proxy((Python3Parser::SuiteContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::TestContext*>(node)) {
    TestContextProxy proxy((Python3Parser::TestContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::TypedargslistContext*>(node)) {
    TypedargslistContextProxy proxy((Python3Parser::TypedargslistContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::TfpdefContext*>(node)) {
    TfpdefContextProxy proxy((Python3Parser::TfpdefContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::VarargslistContext*>(node)) {
    VarargslistContextProxy proxy((Python3Parser::VarargslistContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::VfpdefContext*>(node)) {
    VfpdefContextProxy proxy((Python3Parser::VfpdefContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Small_stmtContext*>(node)) {
    Small_stmtContextProxy proxy((Python3Parser::Small_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Expr_stmtContext*>(node)) {
    Expr_stmtContextProxy proxy((Python3Parser::Expr_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Del_stmtContext*>(node)) {
    Del_stmtContextProxy proxy((Python3Parser::Del_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Pass_stmtContext*>(node)) {
    Pass_stmtContextProxy proxy((Python3Parser::Pass_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Flow_stmtContext*>(node)) {
    Flow_stmtContextProxy proxy((Python3Parser::Flow_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Import_stmtContext*>(node)) {
    Import_stmtContextProxy proxy((Python3Parser::Import_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Global_stmtContext*>(node)) {
    Global_stmtContextProxy proxy((Python3Parser::Global_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Nonlocal_stmtContext*>(node)) {
    Nonlocal_stmtContextProxy proxy((Python3Parser::Nonlocal_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Assert_stmtContext*>(node)) {
    Assert_stmtContextProxy proxy((Python3Parser::Assert_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Testlist_star_exprContext*>(node)) {
    Testlist_star_exprContextProxy proxy((Python3Parser::Testlist_star_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::AnnassignContext*>(node)) {
    AnnassignContextProxy proxy((Python3Parser::AnnassignContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::AugassignContext*>(node)) {
    AugassignContextProxy proxy((Python3Parser::AugassignContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Yield_exprContext*>(node)) {
    Yield_exprContextProxy proxy((Python3Parser::Yield_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Star_exprContext*>(node)) {
    Star_exprContextProxy proxy((Python3Parser::Star_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ExprlistContext*>(node)) {
    ExprlistContextProxy proxy((Python3Parser::ExprlistContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Break_stmtContext*>(node)) {
    Break_stmtContextProxy proxy((Python3Parser::Break_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Continue_stmtContext*>(node)) {
    Continue_stmtContextProxy proxy((Python3Parser::Continue_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Return_stmtContext*>(node)) {
    Return_stmtContextProxy proxy((Python3Parser::Return_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Raise_stmtContext*>(node)) {
    Raise_stmtContextProxy proxy((Python3Parser::Raise_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Yield_stmtContext*>(node)) {
    Yield_stmtContextProxy proxy((Python3Parser::Yield_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Import_nameContext*>(node)) {
    Import_nameContextProxy proxy((Python3Parser::Import_nameContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Import_fromContext*>(node)) {
    Import_fromContextProxy proxy((Python3Parser::Import_fromContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Dotted_as_namesContext*>(node)) {
    Dotted_as_namesContextProxy proxy((Python3Parser::Dotted_as_namesContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Import_as_namesContext*>(node)) {
    Import_as_namesContextProxy proxy((Python3Parser::Import_as_namesContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Import_as_nameContext*>(node)) {
    Import_as_nameContextProxy proxy((Python3Parser::Import_as_nameContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Dotted_as_nameContext*>(node)) {
    Dotted_as_nameContextProxy proxy((Python3Parser::Dotted_as_nameContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::If_stmtContext*>(node)) {
    If_stmtContextProxy proxy((Python3Parser::If_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::While_stmtContext*>(node)) {
    While_stmtContextProxy proxy((Python3Parser::While_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::For_stmtContext*>(node)) {
    For_stmtContextProxy proxy((Python3Parser::For_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Try_stmtContext*>(node)) {
    Try_stmtContextProxy proxy((Python3Parser::Try_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::With_stmtContext*>(node)) {
    With_stmtContextProxy proxy((Python3Parser::With_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Async_stmtContext*>(node)) {
    Async_stmtContextProxy proxy((Python3Parser::Async_stmtContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Except_clauseContext*>(node)) {
    Except_clauseContextProxy proxy((Python3Parser::Except_clauseContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::With_itemContext*>(node)) {
    With_itemContextProxy proxy((Python3Parser::With_itemContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ExprContext*>(node)) {
    ExprContextProxy proxy((Python3Parser::ExprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Or_testContext*>(node)) {
    Or_testContextProxy proxy((Python3Parser::Or_testContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::LambdefContext*>(node)) {
    LambdefContextProxy proxy((Python3Parser::LambdefContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Test_nocondContext*>(node)) {
    Test_nocondContextProxy proxy((Python3Parser::Test_nocondContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Lambdef_nocondContext*>(node)) {
    Lambdef_nocondContextProxy proxy((Python3Parser::Lambdef_nocondContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::And_testContext*>(node)) {
    And_testContextProxy proxy((Python3Parser::And_testContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Not_testContext*>(node)) {
    Not_testContextProxy proxy((Python3Parser::Not_testContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ComparisonContext*>(node)) {
    ComparisonContextProxy proxy((Python3Parser::ComparisonContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Comp_opContext*>(node)) {
    Comp_opContextProxy proxy((Python3Parser::Comp_opContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Xor_exprContext*>(node)) {
    Xor_exprContextProxy proxy((Python3Parser::Xor_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::And_exprContext*>(node)) {
    And_exprContextProxy proxy((Python3Parser::And_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Shift_exprContext*>(node)) {
    Shift_exprContextProxy proxy((Python3Parser::Shift_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Arith_exprContext*>(node)) {
    Arith_exprContextProxy proxy((Python3Parser::Arith_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::TermContext*>(node)) {
    TermContextProxy proxy((Python3Parser::TermContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::FactorContext*>(node)) {
    FactorContextProxy proxy((Python3Parser::FactorContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::PowerContext*>(node)) {
    PowerContextProxy proxy((Python3Parser::PowerContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Atom_exprContext*>(node)) {
    Atom_exprContextProxy proxy((Python3Parser::Atom_exprContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::AtomContext*>(node)) {
    AtomContextProxy proxy((Python3Parser::AtomContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::TrailerContext*>(node)) {
    TrailerContextProxy proxy((Python3Parser::TrailerContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Testlist_compContext*>(node)) {
    Testlist_compContextProxy proxy((Python3Parser::Testlist_compContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::DictorsetmakerContext*>(node)) {
    DictorsetmakerContextProxy proxy((Python3Parser::DictorsetmakerContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Comp_forContext*>(node)) {
    Comp_forContextProxy proxy((Python3Parser::Comp_forContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::SubscriptlistContext*>(node)) {
    SubscriptlistContextProxy proxy((Python3Parser::SubscriptlistContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::SubscriptContext*>(node)) {
    SubscriptContextProxy proxy((Python3Parser::SubscriptContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::SliceopContext*>(node)) {
    SliceopContextProxy proxy((Python3Parser::SliceopContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::ArgumentContext*>(node)) {
    ArgumentContextProxy proxy((Python3Parser::ArgumentContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Comp_iterContext*>(node)) {
    Comp_iterContextProxy proxy((Python3Parser::Comp_iterContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Comp_ifContext*>(node)) {
    Comp_ifContextProxy proxy((Python3Parser::Comp_ifContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Encoding_declContext*>(node)) {
    Encoding_declContextProxy proxy((Python3Parser::Encoding_declContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<Python3Parser::Yield_argContext*>(node)) {
    Yield_argContextProxy proxy((Python3Parser::Yield_argContext*)node);
    return to_ruby(proxy);
  }
  else if (antlrcpp::is<tree::TerminalNodeImpl*>(node)) {
    TerminalNodeProxy proxy(node);
    return to_ruby(proxy);
  }
}


extern "C"
void Init_python3_parser() {
  Module rb_mPython3Parser = define_module("Python3Parser");

  rb_cToken = rb_mPython3Parser
    .define_class<Token>("Token")
    .define_method("text", &Token::getText);

  rb_cParser = rb_mPython3Parser
    .define_class<ParserProxy>("Parser")
    .define_singleton_method("parse", &ParserProxy::parse)
    .define_singleton_method("parse_file", &ParserProxy::parseFile)
    .define_method("visit", &ParserProxy::visit);

  rb_cParseTree = rb_mPython3Parser
    .define_class<tree::ParseTree>("ParseTree");

  rb_mPython3Parser
    .define_class<ContextProxy>("Context")
    .define_method("children", &ContextProxy::getChildren)
    .define_method("child_count", &ContextProxy::childCount)
    .define_method("text", &ContextProxy::getText)
    .define_method("parent", &ContextProxy::getParent);

  rb_cTerminalNode = rb_mPython3Parser
    .define_class<TerminalNodeProxy, ContextProxy>("TerminalNodeImpl");

  rb_mPython3Parser
    .define_class<VisitorProxy>("Visitor")
    .define_director<VisitorProxy>()
    .define_constructor(Constructor<VisitorProxy, Object>())
    .define_method("visit", &VisitorProxy::ruby_visit)
    .define_method("visit_children", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_single_input", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_file_input", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_eval_input", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_decorator", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_decorators", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_decorated", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_async_funcdef", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_funcdef", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_parameters", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_typedargslist", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_tfpdef", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_varargslist", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_vfpdef", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_simple_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_small_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_expr_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_annassign", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_testlist_star_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_augassign", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_del_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_pass_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_flow_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_break_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_continue_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_return_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_yield_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_raise_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_import_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_import_name", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_import_from", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_import_as_name", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_dotted_as_name", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_import_as_names", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_dotted_as_names", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_dotted_name", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_global_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_nonlocal_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_assert_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_compound_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_async_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_if_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_while_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_for_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_try_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_with_stmt", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_with_item", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_except_clause", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_suite", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_test", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_test_nocond", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_lambdef", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_lambdef_nocond", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_or_test", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_and_test", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_not_test", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_comparison", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_comp_op", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_star_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_xor_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_and_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_shift_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_arith_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_term", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_factor", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_power", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_atom_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_atom", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_testlist_comp", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_trailer", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_subscriptlist", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_subscript", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_sliceop", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_exprlist", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_testlist", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_dictorsetmaker", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_classdef", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_arglist", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_argument", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_comp_iter", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_comp_for", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_comp_if", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_encoding_decl", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_yield_expr", &VisitorProxy::ruby_visitChildren)
    .define_method("visit_yield_arg", &VisitorProxy::ruby_visitChildren);

  rb_cSingle_inputContext = rb_mPython3Parser
    .define_class<Single_inputContextProxy, ContextProxy>("Single_inputContext")
    .define_method("simple_stmt", &Single_inputContextProxy::simple_stmt)
    .define_method("compound_stmt", &Single_inputContextProxy::compound_stmt)
    .define_method("NEWLINE", &Single_inputContextProxy::NEWLINE);

  rb_cSimple_stmtContext = rb_mPython3Parser
    .define_class<Simple_stmtContextProxy, ContextProxy>("Simple_stmtContext")
    .define_method("small_stmt", &Simple_stmtContextProxy::small_stmt)
    .define_method("small_stmt_at", &Simple_stmtContextProxy::small_stmtAt)
    .define_method("NEWLINE", &Simple_stmtContextProxy::NEWLINE)
    .define_method("SEMI_COLON", &Simple_stmtContextProxy::SEMI_COLON)
    .define_method("SEMI_COLONAt", &Simple_stmtContextProxy::SEMI_COLON);

  rb_cCompound_stmtContext = rb_mPython3Parser
    .define_class<Compound_stmtContextProxy, ContextProxy>("Compound_stmtContext")
    .define_method("if_stmt", &Compound_stmtContextProxy::if_stmt)
    .define_method("while_stmt", &Compound_stmtContextProxy::while_stmt)
    .define_method("for_stmt", &Compound_stmtContextProxy::for_stmt)
    .define_method("try_stmt", &Compound_stmtContextProxy::try_stmt)
    .define_method("with_stmt", &Compound_stmtContextProxy::with_stmt)
    .define_method("funcdef", &Compound_stmtContextProxy::funcdef)
    .define_method("classdef", &Compound_stmtContextProxy::classdef)
    .define_method("decorated", &Compound_stmtContextProxy::decorated)
    .define_method("async_stmt", &Compound_stmtContextProxy::async_stmt);

  rb_cFile_inputContext = rb_mPython3Parser
    .define_class<File_inputContextProxy, ContextProxy>("File_inputContext")
    .define_method("stmt", &File_inputContextProxy::stmt)
    .define_method("stmt_at", &File_inputContextProxy::stmtAt)
    .define_method("EOF", &File_inputContextProxy::EOF)
    .define_method("NEWLINE", &File_inputContextProxy::NEWLINE)
    .define_method("NEWLINEAt", &File_inputContextProxy::NEWLINE);

  rb_cStmtContext = rb_mPython3Parser
    .define_class<StmtContextProxy, ContextProxy>("StmtContext")
    .define_method("simple_stmt", &StmtContextProxy::simple_stmt)
    .define_method("compound_stmt", &StmtContextProxy::compound_stmt);

  rb_cEval_inputContext = rb_mPython3Parser
    .define_class<Eval_inputContextProxy, ContextProxy>("Eval_inputContext")
    .define_method("testlist", &Eval_inputContextProxy::testlist)
    .define_method("EOF", &Eval_inputContextProxy::EOF)
    .define_method("NEWLINE", &Eval_inputContextProxy::NEWLINE)
    .define_method("NEWLINEAt", &Eval_inputContextProxy::NEWLINE);

  rb_cTestlistContext = rb_mPython3Parser
    .define_class<TestlistContextProxy, ContextProxy>("TestlistContext")
    .define_method("test", &TestlistContextProxy::test)
    .define_method("test_at", &TestlistContextProxy::testAt)
    .define_method("COMMA", &TestlistContextProxy::COMMA)
    .define_method("COMMAAt", &TestlistContextProxy::COMMA);

  rb_cDecoratorContext = rb_mPython3Parser
    .define_class<DecoratorContextProxy, ContextProxy>("DecoratorContext")
    .define_method("dotted_name", &DecoratorContextProxy::dotted_name)
    .define_method("arglist", &DecoratorContextProxy::arglist)
    .define_method("AT", &DecoratorContextProxy::AT)
    .define_method("NEWLINE", &DecoratorContextProxy::NEWLINE)
    .define_method("OPEN_PAREN", &DecoratorContextProxy::OPEN_PAREN)
    .define_method("CLOSE_PAREN", &DecoratorContextProxy::CLOSE_PAREN);

  rb_cDotted_nameContext = rb_mPython3Parser
    .define_class<Dotted_nameContextProxy, ContextProxy>("Dotted_nameContext")
    .define_method("NAME", &Dotted_nameContextProxy::NAME)
    .define_method("NAMEAt", &Dotted_nameContextProxy::NAME)
    .define_method("DOT", &Dotted_nameContextProxy::DOT)
    .define_method("DOTAt", &Dotted_nameContextProxy::DOT);

  rb_cArglistContext = rb_mPython3Parser
    .define_class<ArglistContextProxy, ContextProxy>("ArglistContext")
    .define_method("argument", &ArglistContextProxy::argument)
    .define_method("argument_at", &ArglistContextProxy::argumentAt)
    .define_method("COMMA", &ArglistContextProxy::COMMA)
    .define_method("COMMAAt", &ArglistContextProxy::COMMA);

  rb_cDecoratorsContext = rb_mPython3Parser
    .define_class<DecoratorsContextProxy, ContextProxy>("DecoratorsContext")
    .define_method("decorator", &DecoratorsContextProxy::decorator)
    .define_method("decorator_at", &DecoratorsContextProxy::decoratorAt);

  rb_cDecoratedContext = rb_mPython3Parser
    .define_class<DecoratedContextProxy, ContextProxy>("DecoratedContext")
    .define_method("decorators", &DecoratedContextProxy::decorators)
    .define_method("classdef", &DecoratedContextProxy::classdef)
    .define_method("funcdef", &DecoratedContextProxy::funcdef)
    .define_method("async_funcdef", &DecoratedContextProxy::async_funcdef);

  rb_cClassdefContext = rb_mPython3Parser
    .define_class<ClassdefContextProxy, ContextProxy>("ClassdefContext")
    .define_method("suite", &ClassdefContextProxy::suite)
    .define_method("arglist", &ClassdefContextProxy::arglist)
    .define_method("CLASS", &ClassdefContextProxy::CLASS)
    .define_method("NAME", &ClassdefContextProxy::NAME)
    .define_method("COLON", &ClassdefContextProxy::COLON)
    .define_method("OPEN_PAREN", &ClassdefContextProxy::OPEN_PAREN)
    .define_method("CLOSE_PAREN", &ClassdefContextProxy::CLOSE_PAREN);

  rb_cFuncdefContext = rb_mPython3Parser
    .define_class<FuncdefContextProxy, ContextProxy>("FuncdefContext")
    .define_method("parameters", &FuncdefContextProxy::parameters)
    .define_method("suite", &FuncdefContextProxy::suite)
    .define_method("test", &FuncdefContextProxy::test)
    .define_method("DEF", &FuncdefContextProxy::DEF)
    .define_method("NAME", &FuncdefContextProxy::NAME)
    .define_method("COLON", &FuncdefContextProxy::COLON)
    .define_method("ARROW", &FuncdefContextProxy::ARROW);

  rb_cAsync_funcdefContext = rb_mPython3Parser
    .define_class<Async_funcdefContextProxy, ContextProxy>("Async_funcdefContext")
    .define_method("funcdef", &Async_funcdefContextProxy::funcdef)
    .define_method("ASYNC", &Async_funcdefContextProxy::ASYNC);

  rb_cParametersContext = rb_mPython3Parser
    .define_class<ParametersContextProxy, ContextProxy>("ParametersContext")
    .define_method("typedargslist", &ParametersContextProxy::typedargslist)
    .define_method("OPEN_PAREN", &ParametersContextProxy::OPEN_PAREN)
    .define_method("CLOSE_PAREN", &ParametersContextProxy::CLOSE_PAREN);

  rb_cSuiteContext = rb_mPython3Parser
    .define_class<SuiteContextProxy, ContextProxy>("SuiteContext")
    .define_method("simple_stmt", &SuiteContextProxy::simple_stmt)
    .define_method("stmt", &SuiteContextProxy::stmt)
    .define_method("stmt_at", &SuiteContextProxy::stmtAt)
    .define_method("NEWLINE", &SuiteContextProxy::NEWLINE)
    .define_method("INDENT", &SuiteContextProxy::INDENT)
    .define_method("DEDENT", &SuiteContextProxy::DEDENT);

  rb_cTestContext = rb_mPython3Parser
    .define_class<TestContextProxy, ContextProxy>("TestContext")
    .define_method("or_test", &TestContextProxy::or_test)
    .define_method("or_test_at", &TestContextProxy::or_testAt)
    .define_method("test", &TestContextProxy::test)
    .define_method("lambdef", &TestContextProxy::lambdef)
    .define_method("IF", &TestContextProxy::IF)
    .define_method("ELSE", &TestContextProxy::ELSE);

  rb_cTypedargslistContext = rb_mPython3Parser
    .define_class<TypedargslistContextProxy, ContextProxy>("TypedargslistContext")
    .define_method("tfpdef", &TypedargslistContextProxy::tfpdef)
    .define_method("tfpdef_at", &TypedargslistContextProxy::tfpdefAt)
    .define_method("test", &TypedargslistContextProxy::test)
    .define_method("test_at", &TypedargslistContextProxy::testAt)
    .define_method("STAR", &TypedargslistContextProxy::STAR)
    .define_method("POWER", &TypedargslistContextProxy::POWER)
    .define_method("ASSIGN", &TypedargslistContextProxy::ASSIGN)
    .define_method("ASSIGNAt", &TypedargslistContextProxy::ASSIGN)
    .define_method("COMMA", &TypedargslistContextProxy::COMMA)
    .define_method("COMMAAt", &TypedargslistContextProxy::COMMA);

  rb_cTfpdefContext = rb_mPython3Parser
    .define_class<TfpdefContextProxy, ContextProxy>("TfpdefContext")
    .define_method("test", &TfpdefContextProxy::test)
    .define_method("NAME", &TfpdefContextProxy::NAME)
    .define_method("COLON", &TfpdefContextProxy::COLON);

  rb_cVarargslistContext = rb_mPython3Parser
    .define_class<VarargslistContextProxy, ContextProxy>("VarargslistContext")
    .define_method("vfpdef", &VarargslistContextProxy::vfpdef)
    .define_method("vfpdef_at", &VarargslistContextProxy::vfpdefAt)
    .define_method("test", &VarargslistContextProxy::test)
    .define_method("test_at", &VarargslistContextProxy::testAt)
    .define_method("STAR", &VarargslistContextProxy::STAR)
    .define_method("POWER", &VarargslistContextProxy::POWER)
    .define_method("ASSIGN", &VarargslistContextProxy::ASSIGN)
    .define_method("ASSIGNAt", &VarargslistContextProxy::ASSIGN)
    .define_method("COMMA", &VarargslistContextProxy::COMMA)
    .define_method("COMMAAt", &VarargslistContextProxy::COMMA);

  rb_cVfpdefContext = rb_mPython3Parser
    .define_class<VfpdefContextProxy, ContextProxy>("VfpdefContext")
    .define_method("NAME", &VfpdefContextProxy::NAME);

  rb_cSmall_stmtContext = rb_mPython3Parser
    .define_class<Small_stmtContextProxy, ContextProxy>("Small_stmtContext")
    .define_method("expr_stmt", &Small_stmtContextProxy::expr_stmt)
    .define_method("del_stmt", &Small_stmtContextProxy::del_stmt)
    .define_method("pass_stmt", &Small_stmtContextProxy::pass_stmt)
    .define_method("flow_stmt", &Small_stmtContextProxy::flow_stmt)
    .define_method("import_stmt", &Small_stmtContextProxy::import_stmt)
    .define_method("global_stmt", &Small_stmtContextProxy::global_stmt)
    .define_method("nonlocal_stmt", &Small_stmtContextProxy::nonlocal_stmt)
    .define_method("assert_stmt", &Small_stmtContextProxy::assert_stmt);

  rb_cExpr_stmtContext = rb_mPython3Parser
    .define_class<Expr_stmtContextProxy, ContextProxy>("Expr_stmtContext")
    .define_method("testlist_star_expr", &Expr_stmtContextProxy::testlist_star_expr)
    .define_method("testlist_star_expr_at", &Expr_stmtContextProxy::testlist_star_exprAt)
    .define_method("annassign", &Expr_stmtContextProxy::annassign)
    .define_method("augassign", &Expr_stmtContextProxy::augassign)
    .define_method("yield_expr", &Expr_stmtContextProxy::yield_expr)
    .define_method("yield_expr_at", &Expr_stmtContextProxy::yield_exprAt)
    .define_method("testlist", &Expr_stmtContextProxy::testlist)
    .define_method("ASSIGN", &Expr_stmtContextProxy::ASSIGN)
    .define_method("ASSIGNAt", &Expr_stmtContextProxy::ASSIGN);

  rb_cDel_stmtContext = rb_mPython3Parser
    .define_class<Del_stmtContextProxy, ContextProxy>("Del_stmtContext")
    .define_method("exprlist", &Del_stmtContextProxy::exprlist)
    .define_method("DEL", &Del_stmtContextProxy::DEL);

  rb_cPass_stmtContext = rb_mPython3Parser
    .define_class<Pass_stmtContextProxy, ContextProxy>("Pass_stmtContext")
    .define_method("PASS", &Pass_stmtContextProxy::PASS);

  rb_cFlow_stmtContext = rb_mPython3Parser
    .define_class<Flow_stmtContextProxy, ContextProxy>("Flow_stmtContext")
    .define_method("break_stmt", &Flow_stmtContextProxy::break_stmt)
    .define_method("continue_stmt", &Flow_stmtContextProxy::continue_stmt)
    .define_method("return_stmt", &Flow_stmtContextProxy::return_stmt)
    .define_method("raise_stmt", &Flow_stmtContextProxy::raise_stmt)
    .define_method("yield_stmt", &Flow_stmtContextProxy::yield_stmt);

  rb_cImport_stmtContext = rb_mPython3Parser
    .define_class<Import_stmtContextProxy, ContextProxy>("Import_stmtContext")
    .define_method("import_name", &Import_stmtContextProxy::import_name)
    .define_method("import_from", &Import_stmtContextProxy::import_from);

  rb_cGlobal_stmtContext = rb_mPython3Parser
    .define_class<Global_stmtContextProxy, ContextProxy>("Global_stmtContext")
    .define_method("GLOBAL", &Global_stmtContextProxy::GLOBAL)
    .define_method("NAME", &Global_stmtContextProxy::NAME)
    .define_method("NAMEAt", &Global_stmtContextProxy::NAME)
    .define_method("COMMA", &Global_stmtContextProxy::COMMA)
    .define_method("COMMAAt", &Global_stmtContextProxy::COMMA);

  rb_cNonlocal_stmtContext = rb_mPython3Parser
    .define_class<Nonlocal_stmtContextProxy, ContextProxy>("Nonlocal_stmtContext")
    .define_method("NONLOCAL", &Nonlocal_stmtContextProxy::NONLOCAL)
    .define_method("NAME", &Nonlocal_stmtContextProxy::NAME)
    .define_method("NAMEAt", &Nonlocal_stmtContextProxy::NAME)
    .define_method("COMMA", &Nonlocal_stmtContextProxy::COMMA)
    .define_method("COMMAAt", &Nonlocal_stmtContextProxy::COMMA);

  rb_cAssert_stmtContext = rb_mPython3Parser
    .define_class<Assert_stmtContextProxy, ContextProxy>("Assert_stmtContext")
    .define_method("test", &Assert_stmtContextProxy::test)
    .define_method("test_at", &Assert_stmtContextProxy::testAt)
    .define_method("ASSERT", &Assert_stmtContextProxy::ASSERT)
    .define_method("COMMA", &Assert_stmtContextProxy::COMMA);

  rb_cTestlist_star_exprContext = rb_mPython3Parser
    .define_class<Testlist_star_exprContextProxy, ContextProxy>("Testlist_star_exprContext")
    .define_method("test", &Testlist_star_exprContextProxy::test)
    .define_method("test_at", &Testlist_star_exprContextProxy::testAt)
    .define_method("star_expr", &Testlist_star_exprContextProxy::star_expr)
    .define_method("star_expr_at", &Testlist_star_exprContextProxy::star_exprAt)
    .define_method("COMMA", &Testlist_star_exprContextProxy::COMMA)
    .define_method("COMMAAt", &Testlist_star_exprContextProxy::COMMA);

  rb_cAnnassignContext = rb_mPython3Parser
    .define_class<AnnassignContextProxy, ContextProxy>("AnnassignContext")
    .define_method("test", &AnnassignContextProxy::test)
    .define_method("test_at", &AnnassignContextProxy::testAt)
    .define_method("COLON", &AnnassignContextProxy::COLON)
    .define_method("ASSIGN", &AnnassignContextProxy::ASSIGN);

  rb_cAugassignContext = rb_mPython3Parser
    .define_class<AugassignContextProxy, ContextProxy>("AugassignContext")
    .define_method("ADD_ASSIGN", &AugassignContextProxy::ADD_ASSIGN)
    .define_method("SUB_ASSIGN", &AugassignContextProxy::SUB_ASSIGN)
    .define_method("MULT_ASSIGN", &AugassignContextProxy::MULT_ASSIGN)
    .define_method("AT_ASSIGN", &AugassignContextProxy::AT_ASSIGN)
    .define_method("DIV_ASSIGN", &AugassignContextProxy::DIV_ASSIGN)
    .define_method("MOD_ASSIGN", &AugassignContextProxy::MOD_ASSIGN)
    .define_method("AND_ASSIGN", &AugassignContextProxy::AND_ASSIGN)
    .define_method("OR_ASSIGN", &AugassignContextProxy::OR_ASSIGN)
    .define_method("XOR_ASSIGN", &AugassignContextProxy::XOR_ASSIGN)
    .define_method("LEFT_SHIFT_ASSIGN", &AugassignContextProxy::LEFT_SHIFT_ASSIGN)
    .define_method("RIGHT_SHIFT_ASSIGN", &AugassignContextProxy::RIGHT_SHIFT_ASSIGN)
    .define_method("POWER_ASSIGN", &AugassignContextProxy::POWER_ASSIGN)
    .define_method("IDIV_ASSIGN", &AugassignContextProxy::IDIV_ASSIGN);

  rb_cYield_exprContext = rb_mPython3Parser
    .define_class<Yield_exprContextProxy, ContextProxy>("Yield_exprContext")
    .define_method("yield_arg", &Yield_exprContextProxy::yield_arg)
    .define_method("YIELD", &Yield_exprContextProxy::YIELD);

  rb_cStar_exprContext = rb_mPython3Parser
    .define_class<Star_exprContextProxy, ContextProxy>("Star_exprContext")
    .define_method("expr", &Star_exprContextProxy::expr)
    .define_method("STAR", &Star_exprContextProxy::STAR);

  rb_cExprlistContext = rb_mPython3Parser
    .define_class<ExprlistContextProxy, ContextProxy>("ExprlistContext")
    .define_method("expr", &ExprlistContextProxy::expr)
    .define_method("expr_at", &ExprlistContextProxy::exprAt)
    .define_method("star_expr", &ExprlistContextProxy::star_expr)
    .define_method("star_expr_at", &ExprlistContextProxy::star_exprAt)
    .define_method("COMMA", &ExprlistContextProxy::COMMA)
    .define_method("COMMAAt", &ExprlistContextProxy::COMMA);

  rb_cBreak_stmtContext = rb_mPython3Parser
    .define_class<Break_stmtContextProxy, ContextProxy>("Break_stmtContext")
    .define_method("BREAK", &Break_stmtContextProxy::BREAK);

  rb_cContinue_stmtContext = rb_mPython3Parser
    .define_class<Continue_stmtContextProxy, ContextProxy>("Continue_stmtContext")
    .define_method("CONTINUE", &Continue_stmtContextProxy::CONTINUE);

  rb_cReturn_stmtContext = rb_mPython3Parser
    .define_class<Return_stmtContextProxy, ContextProxy>("Return_stmtContext")
    .define_method("testlist", &Return_stmtContextProxy::testlist)
    .define_method("RETURN", &Return_stmtContextProxy::RETURN);

  rb_cRaise_stmtContext = rb_mPython3Parser
    .define_class<Raise_stmtContextProxy, ContextProxy>("Raise_stmtContext")
    .define_method("test", &Raise_stmtContextProxy::test)
    .define_method("test_at", &Raise_stmtContextProxy::testAt)
    .define_method("RAISE", &Raise_stmtContextProxy::RAISE)
    .define_method("FROM", &Raise_stmtContextProxy::FROM);

  rb_cYield_stmtContext = rb_mPython3Parser
    .define_class<Yield_stmtContextProxy, ContextProxy>("Yield_stmtContext")
    .define_method("yield_expr", &Yield_stmtContextProxy::yield_expr);

  rb_cImport_nameContext = rb_mPython3Parser
    .define_class<Import_nameContextProxy, ContextProxy>("Import_nameContext")
    .define_method("dotted_as_names", &Import_nameContextProxy::dotted_as_names)
    .define_method("IMPORT", &Import_nameContextProxy::IMPORT);

  rb_cImport_fromContext = rb_mPython3Parser
    .define_class<Import_fromContextProxy, ContextProxy>("Import_fromContext")
    .define_method("dotted_name", &Import_fromContextProxy::dotted_name)
    .define_method("import_as_names", &Import_fromContextProxy::import_as_names)
    .define_method("FROM", &Import_fromContextProxy::FROM)
    .define_method("IMPORT", &Import_fromContextProxy::IMPORT)
    .define_method("STAR", &Import_fromContextProxy::STAR)
    .define_method("OPEN_PAREN", &Import_fromContextProxy::OPEN_PAREN)
    .define_method("CLOSE_PAREN", &Import_fromContextProxy::CLOSE_PAREN)
    .define_method("DOT", &Import_fromContextProxy::DOT)
    .define_method("DOTAt", &Import_fromContextProxy::DOT)
    .define_method("ELLIPSIS", &Import_fromContextProxy::ELLIPSIS)
    .define_method("ELLIPSISAt", &Import_fromContextProxy::ELLIPSIS);

  rb_cDotted_as_namesContext = rb_mPython3Parser
    .define_class<Dotted_as_namesContextProxy, ContextProxy>("Dotted_as_namesContext")
    .define_method("dotted_as_name", &Dotted_as_namesContextProxy::dotted_as_name)
    .define_method("dotted_as_name_at", &Dotted_as_namesContextProxy::dotted_as_nameAt)
    .define_method("COMMA", &Dotted_as_namesContextProxy::COMMA)
    .define_method("COMMAAt", &Dotted_as_namesContextProxy::COMMA);

  rb_cImport_as_namesContext = rb_mPython3Parser
    .define_class<Import_as_namesContextProxy, ContextProxy>("Import_as_namesContext")
    .define_method("import_as_name", &Import_as_namesContextProxy::import_as_name)
    .define_method("import_as_name_at", &Import_as_namesContextProxy::import_as_nameAt)
    .define_method("COMMA", &Import_as_namesContextProxy::COMMA)
    .define_method("COMMAAt", &Import_as_namesContextProxy::COMMA);

  rb_cImport_as_nameContext = rb_mPython3Parser
    .define_class<Import_as_nameContextProxy, ContextProxy>("Import_as_nameContext")
    .define_method("NAME", &Import_as_nameContextProxy::NAME)
    .define_method("NAMEAt", &Import_as_nameContextProxy::NAME)
    .define_method("AS", &Import_as_nameContextProxy::AS);

  rb_cDotted_as_nameContext = rb_mPython3Parser
    .define_class<Dotted_as_nameContextProxy, ContextProxy>("Dotted_as_nameContext")
    .define_method("dotted_name", &Dotted_as_nameContextProxy::dotted_name)
    .define_method("AS", &Dotted_as_nameContextProxy::AS)
    .define_method("NAME", &Dotted_as_nameContextProxy::NAME);

  rb_cIf_stmtContext = rb_mPython3Parser
    .define_class<If_stmtContextProxy, ContextProxy>("If_stmtContext")
    .define_method("test", &If_stmtContextProxy::test)
    .define_method("test_at", &If_stmtContextProxy::testAt)
    .define_method("suite", &If_stmtContextProxy::suite)
    .define_method("suite_at", &If_stmtContextProxy::suiteAt)
    .define_method("IF", &If_stmtContextProxy::IF)
    .define_method("COLON", &If_stmtContextProxy::COLON)
    .define_method("COLONAt", &If_stmtContextProxy::COLON)
    .define_method("ELIF", &If_stmtContextProxy::ELIF)
    .define_method("ELIFAt", &If_stmtContextProxy::ELIF)
    .define_method("ELSE", &If_stmtContextProxy::ELSE);

  rb_cWhile_stmtContext = rb_mPython3Parser
    .define_class<While_stmtContextProxy, ContextProxy>("While_stmtContext")
    .define_method("test", &While_stmtContextProxy::test)
    .define_method("suite", &While_stmtContextProxy::suite)
    .define_method("suite_at", &While_stmtContextProxy::suiteAt)
    .define_method("WHILE", &While_stmtContextProxy::WHILE)
    .define_method("COLON", &While_stmtContextProxy::COLON)
    .define_method("COLONAt", &While_stmtContextProxy::COLON)
    .define_method("ELSE", &While_stmtContextProxy::ELSE);

  rb_cFor_stmtContext = rb_mPython3Parser
    .define_class<For_stmtContextProxy, ContextProxy>("For_stmtContext")
    .define_method("exprlist", &For_stmtContextProxy::exprlist)
    .define_method("testlist", &For_stmtContextProxy::testlist)
    .define_method("suite", &For_stmtContextProxy::suite)
    .define_method("suite_at", &For_stmtContextProxy::suiteAt)
    .define_method("FOR", &For_stmtContextProxy::FOR)
    .define_method("IN", &For_stmtContextProxy::IN)
    .define_method("COLON", &For_stmtContextProxy::COLON)
    .define_method("COLONAt", &For_stmtContextProxy::COLON)
    .define_method("ELSE", &For_stmtContextProxy::ELSE);

  rb_cTry_stmtContext = rb_mPython3Parser
    .define_class<Try_stmtContextProxy, ContextProxy>("Try_stmtContext")
    .define_method("suite", &Try_stmtContextProxy::suite)
    .define_method("suite_at", &Try_stmtContextProxy::suiteAt)
    .define_method("except_clause", &Try_stmtContextProxy::except_clause)
    .define_method("except_clause_at", &Try_stmtContextProxy::except_clauseAt)
    .define_method("TRY", &Try_stmtContextProxy::TRY)
    .define_method("COLON", &Try_stmtContextProxy::COLON)
    .define_method("COLONAt", &Try_stmtContextProxy::COLON)
    .define_method("FINALLY", &Try_stmtContextProxy::FINALLY)
    .define_method("ELSE", &Try_stmtContextProxy::ELSE);

  rb_cWith_stmtContext = rb_mPython3Parser
    .define_class<With_stmtContextProxy, ContextProxy>("With_stmtContext")
    .define_method("with_item", &With_stmtContextProxy::with_item)
    .define_method("with_item_at", &With_stmtContextProxy::with_itemAt)
    .define_method("suite", &With_stmtContextProxy::suite)
    .define_method("WITH", &With_stmtContextProxy::WITH)
    .define_method("COLON", &With_stmtContextProxy::COLON)
    .define_method("COMMA", &With_stmtContextProxy::COMMA)
    .define_method("COMMAAt", &With_stmtContextProxy::COMMA);

  rb_cAsync_stmtContext = rb_mPython3Parser
    .define_class<Async_stmtContextProxy, ContextProxy>("Async_stmtContext")
    .define_method("funcdef", &Async_stmtContextProxy::funcdef)
    .define_method("with_stmt", &Async_stmtContextProxy::with_stmt)
    .define_method("for_stmt", &Async_stmtContextProxy::for_stmt)
    .define_method("ASYNC", &Async_stmtContextProxy::ASYNC);

  rb_cExcept_clauseContext = rb_mPython3Parser
    .define_class<Except_clauseContextProxy, ContextProxy>("Except_clauseContext")
    .define_method("test", &Except_clauseContextProxy::test)
    .define_method("EXCEPT", &Except_clauseContextProxy::EXCEPT)
    .define_method("AS", &Except_clauseContextProxy::AS)
    .define_method("NAME", &Except_clauseContextProxy::NAME);

  rb_cWith_itemContext = rb_mPython3Parser
    .define_class<With_itemContextProxy, ContextProxy>("With_itemContext")
    .define_method("test", &With_itemContextProxy::test)
    .define_method("expr", &With_itemContextProxy::expr)
    .define_method("AS", &With_itemContextProxy::AS);

  rb_cExprContext = rb_mPython3Parser
    .define_class<ExprContextProxy, ContextProxy>("ExprContext")
    .define_method("xor_expr", &ExprContextProxy::xor_expr)
    .define_method("xor_expr_at", &ExprContextProxy::xor_exprAt)
    .define_method("OR_OP", &ExprContextProxy::OR_OP)
    .define_method("OR_OPAt", &ExprContextProxy::OR_OP);

  rb_cOr_testContext = rb_mPython3Parser
    .define_class<Or_testContextProxy, ContextProxy>("Or_testContext")
    .define_method("and_test", &Or_testContextProxy::and_test)
    .define_method("and_test_at", &Or_testContextProxy::and_testAt)
    .define_method("OR", &Or_testContextProxy::OR)
    .define_method("ORAt", &Or_testContextProxy::OR);

  rb_cLambdefContext = rb_mPython3Parser
    .define_class<LambdefContextProxy, ContextProxy>("LambdefContext")
    .define_method("test", &LambdefContextProxy::test)
    .define_method("varargslist", &LambdefContextProxy::varargslist)
    .define_method("LAMBDA", &LambdefContextProxy::LAMBDA)
    .define_method("COLON", &LambdefContextProxy::COLON);

  rb_cTest_nocondContext = rb_mPython3Parser
    .define_class<Test_nocondContextProxy, ContextProxy>("Test_nocondContext")
    .define_method("or_test", &Test_nocondContextProxy::or_test)
    .define_method("lambdef_nocond", &Test_nocondContextProxy::lambdef_nocond);

  rb_cLambdef_nocondContext = rb_mPython3Parser
    .define_class<Lambdef_nocondContextProxy, ContextProxy>("Lambdef_nocondContext")
    .define_method("test_nocond", &Lambdef_nocondContextProxy::test_nocond)
    .define_method("varargslist", &Lambdef_nocondContextProxy::varargslist)
    .define_method("LAMBDA", &Lambdef_nocondContextProxy::LAMBDA)
    .define_method("COLON", &Lambdef_nocondContextProxy::COLON);

  rb_cAnd_testContext = rb_mPython3Parser
    .define_class<And_testContextProxy, ContextProxy>("And_testContext")
    .define_method("not_test", &And_testContextProxy::not_test)
    .define_method("not_test_at", &And_testContextProxy::not_testAt)
    .define_method("AND", &And_testContextProxy::AND)
    .define_method("ANDAt", &And_testContextProxy::AND);

  rb_cNot_testContext = rb_mPython3Parser
    .define_class<Not_testContextProxy, ContextProxy>("Not_testContext")
    .define_method("not_test", &Not_testContextProxy::not_test)
    .define_method("comparison", &Not_testContextProxy::comparison)
    .define_method("NOT", &Not_testContextProxy::NOT);

  rb_cComparisonContext = rb_mPython3Parser
    .define_class<ComparisonContextProxy, ContextProxy>("ComparisonContext")
    .define_method("expr", &ComparisonContextProxy::expr)
    .define_method("expr_at", &ComparisonContextProxy::exprAt)
    .define_method("comp_op", &ComparisonContextProxy::comp_op)
    .define_method("comp_op_at", &ComparisonContextProxy::comp_opAt);

  rb_cComp_opContext = rb_mPython3Parser
    .define_class<Comp_opContextProxy, ContextProxy>("Comp_opContext")
    .define_method("LESS_THAN", &Comp_opContextProxy::LESS_THAN)
    .define_method("GREATER_THAN", &Comp_opContextProxy::GREATER_THAN)
    .define_method("EQUALS", &Comp_opContextProxy::EQUALS)
    .define_method("GT_EQ", &Comp_opContextProxy::GT_EQ)
    .define_method("LT_EQ", &Comp_opContextProxy::LT_EQ)
    .define_method("NOT_EQ_1", &Comp_opContextProxy::NOT_EQ_1)
    .define_method("NOT_EQ_2", &Comp_opContextProxy::NOT_EQ_2)
    .define_method("IN", &Comp_opContextProxy::IN)
    .define_method("NOT", &Comp_opContextProxy::NOT)
    .define_method("IS", &Comp_opContextProxy::IS);

  rb_cXor_exprContext = rb_mPython3Parser
    .define_class<Xor_exprContextProxy, ContextProxy>("Xor_exprContext")
    .define_method("and_expr", &Xor_exprContextProxy::and_expr)
    .define_method("and_expr_at", &Xor_exprContextProxy::and_exprAt)
    .define_method("XOR", &Xor_exprContextProxy::XOR)
    .define_method("XORAt", &Xor_exprContextProxy::XOR);

  rb_cAnd_exprContext = rb_mPython3Parser
    .define_class<And_exprContextProxy, ContextProxy>("And_exprContext")
    .define_method("shift_expr", &And_exprContextProxy::shift_expr)
    .define_method("shift_expr_at", &And_exprContextProxy::shift_exprAt)
    .define_method("AND_OP", &And_exprContextProxy::AND_OP)
    .define_method("AND_OPAt", &And_exprContextProxy::AND_OP);

  rb_cShift_exprContext = rb_mPython3Parser
    .define_class<Shift_exprContextProxy, ContextProxy>("Shift_exprContext")
    .define_method("arith_expr", &Shift_exprContextProxy::arith_expr)
    .define_method("arith_expr_at", &Shift_exprContextProxy::arith_exprAt)
    .define_method("LEFT_SHIFT", &Shift_exprContextProxy::LEFT_SHIFT)
    .define_method("LEFT_SHIFTAt", &Shift_exprContextProxy::LEFT_SHIFT)
    .define_method("RIGHT_SHIFT", &Shift_exprContextProxy::RIGHT_SHIFT)
    .define_method("RIGHT_SHIFTAt", &Shift_exprContextProxy::RIGHT_SHIFT);

  rb_cArith_exprContext = rb_mPython3Parser
    .define_class<Arith_exprContextProxy, ContextProxy>("Arith_exprContext")
    .define_method("term", &Arith_exprContextProxy::term)
    .define_method("term_at", &Arith_exprContextProxy::termAt)
    .define_method("ADD", &Arith_exprContextProxy::ADD)
    .define_method("ADDAt", &Arith_exprContextProxy::ADD)
    .define_method("MINUS", &Arith_exprContextProxy::MINUS)
    .define_method("MINUSAt", &Arith_exprContextProxy::MINUS);

  rb_cTermContext = rb_mPython3Parser
    .define_class<TermContextProxy, ContextProxy>("TermContext")
    .define_method("factor", &TermContextProxy::factor)
    .define_method("factor_at", &TermContextProxy::factorAt)
    .define_method("STAR", &TermContextProxy::STAR)
    .define_method("STARAt", &TermContextProxy::STAR)
    .define_method("AT", &TermContextProxy::AT)
    .define_method("ATAt", &TermContextProxy::AT)
    .define_method("DIV", &TermContextProxy::DIV)
    .define_method("DIVAt", &TermContextProxy::DIV)
    .define_method("MOD", &TermContextProxy::MOD)
    .define_method("MODAt", &TermContextProxy::MOD)
    .define_method("IDIV", &TermContextProxy::IDIV)
    .define_method("IDIVAt", &TermContextProxy::IDIV);

  rb_cFactorContext = rb_mPython3Parser
    .define_class<FactorContextProxy, ContextProxy>("FactorContext")
    .define_method("factor", &FactorContextProxy::factor)
    .define_method("power", &FactorContextProxy::power)
    .define_method("ADD", &FactorContextProxy::ADD)
    .define_method("MINUS", &FactorContextProxy::MINUS)
    .define_method("NOT_OP", &FactorContextProxy::NOT_OP);

  rb_cPowerContext = rb_mPython3Parser
    .define_class<PowerContextProxy, ContextProxy>("PowerContext")
    .define_method("atom_expr", &PowerContextProxy::atom_expr)
    .define_method("factor", &PowerContextProxy::factor)
    .define_method("POWER", &PowerContextProxy::POWER);

  rb_cAtom_exprContext = rb_mPython3Parser
    .define_class<Atom_exprContextProxy, ContextProxy>("Atom_exprContext")
    .define_method("atom", &Atom_exprContextProxy::atom)
    .define_method("trailer", &Atom_exprContextProxy::trailer)
    .define_method("trailer_at", &Atom_exprContextProxy::trailerAt)
    .define_method("AWAIT", &Atom_exprContextProxy::AWAIT);

  rb_cAtomContext = rb_mPython3Parser
    .define_class<AtomContextProxy, ContextProxy>("AtomContext")
    .define_method("yield_expr", &AtomContextProxy::yield_expr)
    .define_method("testlist_comp", &AtomContextProxy::testlist_comp)
    .define_method("dictorsetmaker", &AtomContextProxy::dictorsetmaker)
    .define_method("OPEN_PAREN", &AtomContextProxy::OPEN_PAREN)
    .define_method("CLOSE_PAREN", &AtomContextProxy::CLOSE_PAREN)
    .define_method("OPEN_BRACK", &AtomContextProxy::OPEN_BRACK)
    .define_method("CLOSE_BRACK", &AtomContextProxy::CLOSE_BRACK)
    .define_method("OPEN_BRACE", &AtomContextProxy::OPEN_BRACE)
    .define_method("CLOSE_BRACE", &AtomContextProxy::CLOSE_BRACE)
    .define_method("NAME", &AtomContextProxy::NAME)
    .define_method("NUMBER", &AtomContextProxy::NUMBER)
    .define_method("ELLIPSIS", &AtomContextProxy::ELLIPSIS)
    .define_method("NONE", &AtomContextProxy::NONE)
    .define_method("TRUE", &AtomContextProxy::TRUE)
    .define_method("FALSE", &AtomContextProxy::FALSE)
    .define_method("STRING", &AtomContextProxy::STRING)
    .define_method("STRINGAt", &AtomContextProxy::STRING);

  rb_cTrailerContext = rb_mPython3Parser
    .define_class<TrailerContextProxy, ContextProxy>("TrailerContext")
    .define_method("arglist", &TrailerContextProxy::arglist)
    .define_method("subscriptlist", &TrailerContextProxy::subscriptlist)
    .define_method("OPEN_PAREN", &TrailerContextProxy::OPEN_PAREN)
    .define_method("CLOSE_PAREN", &TrailerContextProxy::CLOSE_PAREN)
    .define_method("OPEN_BRACK", &TrailerContextProxy::OPEN_BRACK)
    .define_method("CLOSE_BRACK", &TrailerContextProxy::CLOSE_BRACK)
    .define_method("DOT", &TrailerContextProxy::DOT)
    .define_method("NAME", &TrailerContextProxy::NAME);

  rb_cTestlist_compContext = rb_mPython3Parser
    .define_class<Testlist_compContextProxy, ContextProxy>("Testlist_compContext")
    .define_method("test", &Testlist_compContextProxy::test)
    .define_method("test_at", &Testlist_compContextProxy::testAt)
    .define_method("star_expr", &Testlist_compContextProxy::star_expr)
    .define_method("star_expr_at", &Testlist_compContextProxy::star_exprAt)
    .define_method("comp_for", &Testlist_compContextProxy::comp_for)
    .define_method("COMMA", &Testlist_compContextProxy::COMMA)
    .define_method("COMMAAt", &Testlist_compContextProxy::COMMA);

  rb_cDictorsetmakerContext = rb_mPython3Parser
    .define_class<DictorsetmakerContextProxy, ContextProxy>("DictorsetmakerContext")
    .define_method("test", &DictorsetmakerContextProxy::test)
    .define_method("test_at", &DictorsetmakerContextProxy::testAt)
    .define_method("expr", &DictorsetmakerContextProxy::expr)
    .define_method("expr_at", &DictorsetmakerContextProxy::exprAt)
    .define_method("comp_for", &DictorsetmakerContextProxy::comp_for)
    .define_method("star_expr", &DictorsetmakerContextProxy::star_expr)
    .define_method("star_expr_at", &DictorsetmakerContextProxy::star_exprAt)
    .define_method("COLON", &DictorsetmakerContextProxy::COLON)
    .define_method("COLONAt", &DictorsetmakerContextProxy::COLON)
    .define_method("POWER", &DictorsetmakerContextProxy::POWER)
    .define_method("POWERAt", &DictorsetmakerContextProxy::POWER)
    .define_method("COMMA", &DictorsetmakerContextProxy::COMMA)
    .define_method("COMMAAt", &DictorsetmakerContextProxy::COMMA);

  rb_cComp_forContext = rb_mPython3Parser
    .define_class<Comp_forContextProxy, ContextProxy>("Comp_forContext")
    .define_method("exprlist", &Comp_forContextProxy::exprlist)
    .define_method("or_test", &Comp_forContextProxy::or_test)
    .define_method("comp_iter", &Comp_forContextProxy::comp_iter)
    .define_method("FOR", &Comp_forContextProxy::FOR)
    .define_method("IN", &Comp_forContextProxy::IN)
    .define_method("ASYNC", &Comp_forContextProxy::ASYNC);

  rb_cSubscriptlistContext = rb_mPython3Parser
    .define_class<SubscriptlistContextProxy, ContextProxy>("SubscriptlistContext")
    .define_method("subscript", &SubscriptlistContextProxy::subscript)
    .define_method("subscript_at", &SubscriptlistContextProxy::subscriptAt)
    .define_method("COMMA", &SubscriptlistContextProxy::COMMA)
    .define_method("COMMAAt", &SubscriptlistContextProxy::COMMA);

  rb_cSubscriptContext = rb_mPython3Parser
    .define_class<SubscriptContextProxy, ContextProxy>("SubscriptContext")
    .define_method("test", &SubscriptContextProxy::test)
    .define_method("test_at", &SubscriptContextProxy::testAt)
    .define_method("sliceop", &SubscriptContextProxy::sliceop)
    .define_method("COLON", &SubscriptContextProxy::COLON);

  rb_cSliceopContext = rb_mPython3Parser
    .define_class<SliceopContextProxy, ContextProxy>("SliceopContext")
    .define_method("test", &SliceopContextProxy::test)
    .define_method("COLON", &SliceopContextProxy::COLON);

  rb_cArgumentContext = rb_mPython3Parser
    .define_class<ArgumentContextProxy, ContextProxy>("ArgumentContext")
    .define_method("test", &ArgumentContextProxy::test)
    .define_method("test_at", &ArgumentContextProxy::testAt)
    .define_method("comp_for", &ArgumentContextProxy::comp_for)
    .define_method("ASSIGN", &ArgumentContextProxy::ASSIGN)
    .define_method("POWER", &ArgumentContextProxy::POWER)
    .define_method("STAR", &ArgumentContextProxy::STAR);

  rb_cComp_iterContext = rb_mPython3Parser
    .define_class<Comp_iterContextProxy, ContextProxy>("Comp_iterContext")
    .define_method("comp_for", &Comp_iterContextProxy::comp_for)
    .define_method("comp_if", &Comp_iterContextProxy::comp_if);

  rb_cComp_ifContext = rb_mPython3Parser
    .define_class<Comp_ifContextProxy, ContextProxy>("Comp_ifContext")
    .define_method("test_nocond", &Comp_ifContextProxy::test_nocond)
    .define_method("comp_iter", &Comp_ifContextProxy::comp_iter)
    .define_method("IF", &Comp_ifContextProxy::IF);

  rb_cEncoding_declContext = rb_mPython3Parser
    .define_class<Encoding_declContextProxy, ContextProxy>("Encoding_declContext")
    .define_method("NAME", &Encoding_declContextProxy::NAME);

  rb_cYield_argContext = rb_mPython3Parser
    .define_class<Yield_argContextProxy, ContextProxy>("Yield_argContext")
    .define_method("test", &Yield_argContextProxy::test)
    .define_method("testlist", &Yield_argContextProxy::testlist)
    .define_method("FROM", &Yield_argContextProxy::FROM);
}

