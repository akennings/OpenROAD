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

#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <set>
#include <vector>
#include "architecture.h"
#include "detailed_objective.h"
#include "network.h"
#include "router.h"

namespace dpo {

class DetailedOrient;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DetailedABU : public DetailedObjective {
  // This class maintains the ABU metric which can be used as part of a cost
  // function for detailed improvement.
 public:
  DetailedABU(Architecture* arch, Network* network, RoutingParams* rt);
  virtual ~DetailedABU();

  // Those that must be overridden.
  double curr();
  double delta(int n, std::vector<Node*>& nodes, 
               std::vector<int>& curLeft, std::vector<int>& curBottom, 
               std::vector<unsigned>& curOri,
               std::vector<int>& newLeft, std::vector<int>& newBottom,
               std::vector<unsigned>& newOri);
  void accept();
  void reject();

  // Other.
  void init(DetailedMgr* mgr, DetailedOrient* orient);
  void init();
  double calculateABU(bool print = false);
  double measureABU(bool print = false);

  void updateBins(Node* nd, double x, double y, int addSub);
  void acceptBins();
  void rejectBins();
  void clearBins();

  double delta();

  double freeSpaceThreshold();
  double binAreaThreshold();
  double alpha();

  void computeUtils();
  void clearUtils();

  void computeBuckets();
  void clearBuckets();
  int getBucketId(int binId, double occ);

 protected:
  struct density_bin {
    int id;
    double lx, hx;      // low/high x coordinate
    double ly, hy;      // low/high y coordinate
    double area;        // bin area
    double m_util;      // bin's movable cell area
    double c_util;      // bin's old movable cell area
    double f_util;      // bin's fixed cell area
    double free_space;  // bin's freespace area
  };

  DetailedMgr* m_mgrPtr;
  DetailedOrient* m_orientPtr;

  Architecture* m_arch;
  Network* m_network;
  RoutingParams* m_rt;

 public:
  // Utilization monitoring for ABU (if paying attention to ABU).
  std::vector<density_bin> m_abuBins;
  double m_abuGridUnit;
  int m_abuGridNumX;
  int m_abuGridNumY;
  int m_abuNumBins;

  double m_abuTargUt;
  double m_abuTargUt02;
  double m_abuTargUt05;
  double m_abuTargUt10;
  double m_abuTargUt20;

  int m_abuChangedBinsCounter;
  std::vector<int> m_abuChangedBins;
  std::vector<int> m_abuChangedBinsMask;

  std::vector<std::set<int> > m_utilBuckets;
  std::vector<double> m_utilTotals;
};

}  // namespace dpo
