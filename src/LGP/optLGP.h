#include "manipulationTree.h"

typedef ManipulationTree_Node MNode;
typedef ManipulationTree_NodeL MNodeL;
struct OrsPathViewer;


struct OptLGP{
  MNode *root, *displayFocus;

  mlr::Array<OrsPathViewer*> views; //displays for the 3 different levels

  //-- these are lists or queues; I don't maintain them sorted because their evaluation (e.g. f(n)=g(n)+h(n)) changes continuously
  // while new bounds are computed. Therefore, whenever I pop from these lists, I find the minimum w.r.t. a heuristic. The
  // heuristics are defined in main.cpp currently
  MNodeL fringe_expand;//list of nodes to be expanded next
  MNodeL terminals;    //list of found terminals

  MNodeL fringe_pose;  //list of nodes that can be pose tested (parent has been tested)
  MNodeL fringe_pose2; //list of nodes towards a terminal -> scheduled for pose testing
  MNodeL fringe_seq;   //list of terminal nodes that have been pose tested
  MNodeL fringe_path;  //list of terminal nodes that have been seq tested
  MNodeL fringe_done;  //list of terminal nodes that have been path tested

  OptLGP(mlr::KinematicWorld& kin, FOL_World& fol);

  //-- for methods called in the run loop
private:
  MNode* getBest(MNodeL& fringe, uint level);
  MNode* popBest(MNodeL& fringe, uint level);
  void expandBest();
  void optBestOnLevel(int level, MNodeL& fringe, MNodeL* addIfTerminal, MNodeL* addChildren);
  void optFirstOnLevel(int level, MNodeL& fringe, MNodeL* addIfTerminal);
  void clearFromInfeasibles(MNodeL& fringe);
public:
  void run(int verbose, bool display);

  // output
  mlr::String report();
  void updateDisplay();

  //-- kind of a gui:
  void printChoices();
  mlr::String queryForChoice();
  bool execChoice(mlr::String cmd);
  bool execRandomChoice();

};