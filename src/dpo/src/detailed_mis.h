///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2021, Andrew Kennings
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes.
////////////////////////////////////////////////////////////////////////////////
#include <bitset>
#include <cmath>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>
#include "architecture.h"
#include "network.h"
#include "router.h"

namespace dpo {

////////////////////////////////////////////////////////////////////////////////
// Forward declarations.
////////////////////////////////////////////////////////////////////////////////
class DetailedMisParams;
class DetailedMis;
class DetailedSeg;
class DetailedMgr;

////////////////////////////////////////////////////////////////////////////////
// Classes.
////////////////////////////////////////////////////////////////////////////////
class DetailedMisParams {
 public:
  enum Strategy {
    KDTree = 0,
    Binning = 1,
    Colour = 2,
  };

 public:
  DetailedMisParams()
      : _maxDifferenceInMetric(0.03),
        _maxNumNodes(15),
        _maxPasses(1),
        _sizeTol(1.10),
        _skipNetsLargerThanThis(50),
        _strategy(Binning),
        _useSameSize(true)
  // ************************************************************
  {
    ;
  }

 public:
  double _maxDifferenceInMetric;  // How much we allow the routine to
                                  // reintroduce overlap into placement
  unsigned _maxNumNodes;  // Only consider this many number of nodes for B&B (<=
                          // MAX_BB_NODES)
  unsigned _maxPasses;    // Maximum number of B&B passes
  double _sizeTol;        // Tolerance for what is considered same-size
  unsigned _skipNetsLargerThanThis;  // Skip nets larger than this amount.
  Strategy _strategy;                // The type of strategy to consider
  bool _useSameSize;  // If 'false', cells can swap with approximately same-size
                      // locations
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DetailedMis {
  // Flow-based solver for replacing nodes using matching.
  enum Objective { Hpwl, Disp };

 public:
  DetailedMis(Architecture* arch, Network* network, RoutingParams* rt);
  virtual ~DetailedMis();

  void run(DetailedMgr* mgrPtr, std::string command);
  void run(DetailedMgr* mgrPtr, std::vector<std::string>& args);

  /*
  public:
          void				Place( void );
          void 				Place( std::vector<Node *> &,
  std::vector<SwapLocation *> &, unsigned );
          void				SetNetlist( Netlist * );
  */
 protected:
  class Bucket;

  /*
          class BBox;
          struct NodeBin;
          struct SortNodesByArea;

          void 				binTheNodes( std::vector<Node *> & );
          void 				binTheNodes( unsigned, unsigned );
          void				calculateCOG( void );
          void				colourNodes( void );
          void				gatherNodesWithColour( void );
          void 				gatherNodesWithOutwardScan( void );
          void 				gatherNodesWithWindow( void );
          double 				getCostOfPlacement( void );
          double 				getDetailedMisCost( Node *,
     SwapLocation * );
          double 				init( std::vector<Node *> & );
          bool        			isSameSize( Node *, SwapLocation * );
          bool       			isSameSize( Node *, Node * );
          void				place( std::vector<Node *> &,
     std::vector<SwapLocation *> &, unsigned, std::deque<SwapLocation *> & );
   */

  void place();
  void collectMovableCells();
  void colorCells();
  void buildGrid();
  void clearGrid();
  void populateGrid();
  bool gatherNeighbours(Node* ndi);
  void solveMatch();
  double getHpwl(Node* ndi, double xi, double yi);
  double getDisp(Node* ndi, double xi, double yi);

 public:
  /*
          DetailedMisParams			_params;
      */

  DetailedMgr* m_mgrPtr;

  Architecture* m_arch;
  Network* m_network;
  RoutingParams* m_rt;

  std::vector<Node*> m_candidates;
  std::vector<bool> m_movable;
  std::vector<int> m_colors;
  std::vector<Node*> m_neighbours;

  // Grid used for binning and locating cells.
  std::vector<std::vector<Bucket*> > m_grid;
  int m_dimW;
  int m_dimH;
  double m_stepX;
  double m_stepY;
  std::map<Node*, Bucket*> m_cellToBinMap;

  std::vector<int> m_timesUsed;

  // Other.
  int m_skipEdgesLargerThanThis;
  int m_maxProblemSize;
  int m_traversal;
  bool m_useSameSize;
  bool m_useSameColor;
  int m_maxTimesUsed;
  Objective m_obj;
};

}  // namespace dpo
