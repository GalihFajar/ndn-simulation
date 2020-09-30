/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "../src/ndnSIM/helper/ndn-link-control-helper.hpp"
#include <bits/stdc++.h>

namespace ns3 {

int
main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);
  bool isIPSimulation = false; //change to true if IPSimulation is desired
  bool isPriorityFIFO = false; //change to true if PriorityFifo is desired

  AnnotatedTopologyReader topologyReader("", 26);
  topologyReader.SetFileName("src/ndnSIM/examples/inherent-topology.txt");
  topologyReader.Read();

  /****************************************************************************/
  // Install NDN stack on all nodes
  int RouterCSSize = 1000;
  string csPolicy= "nfd::cs::lru";
  if(isIPSimulation) RouterCSSize = 1;
  if(isPriorityFIFO) csPolicy = "nfd::cs::priority_fifo";
  ndn::StackHelper ndnHelper;
  ndnHelper.setPolicy(csPolicy);
  ndnHelper.setCsSize(RouterCSSize);
  ndnHelper.InstallAll();
  /****************************************************************************/
  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();
  /****************************************************************************/
  // Getting containers for the consumer/producer
  Ptr<Node> consumer1 = Names::Find<Node>("Src1");
  Ptr<Node> consumer2 = Names::Find<Node>("Src3");
  Ptr<Node> consumer3 = Names::Find<Node>("Src4");
  Ptr<Node> consumer4 = Names::Find<Node>("Rtr1");
  Ptr<Node> consumer5 = Names::Find<Node>("Rtr3");
  Ptr<Node> consumer6 = Names::Find<Node>("Src2");
  Ptr<Node> consumer7 = Names::Find<Node>("Rtr11");
  Ptr<Node> consumer8 = Names::Find<Node>("Rtr13");
  Ptr<Node> consumer9 = Names::Find<Node>("Rtr17");
  Ptr<Node> consumer10 = Names::Find<Node>("Rtr19");

  Ptr<Node> producer1 = Names::Find<Node>("Dst1");
  Ptr<Node> producer2 = Names::Find<Node>("Dst2");
  /****************************************************************************/
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetAttribute("Frequency", StringValue("100")); // interests per Second
  consumerHelper.SetAttribute("Randomize", StringValue("uniform"));
  /****************************************************************************/
  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer1);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer2);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer3);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer4);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer5);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer6);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer7);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer8);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer9);

  consumerHelper.SetPrefix("/source");
  consumerHelper.Install(consumer10);
  

  /****************************************************************************/
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  /****************************************************************************/
  ndnGlobalRoutingHelper.AddOrigins("/source", producer1);
  producerHelper.SetPrefix("/source");
  producerHelper.Install(producer1);
  
  ndnGlobalRoutingHelper.AddOrigins("/source", producer2);
  producerHelper.SetPrefix("/source");
  producerHelper.Install(producer2);

  //ndnGlobalRoutingHelper.AddOrigins("/source", producer3);
  //producerHelper.SetPrefix("/source");
  //producerHelper.Install(producer3);

  /*****************************************************************************/
  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  // Link between Rtr12 and Dst3 will fail for 3 seconds
  Simulator::Schedule(Seconds(5.0), ndn::LinkControlHelper::FailLinkByName, "Rtr12", "Dst3");
  Simulator::Schedule(Seconds(8.0), ndn::LinkControlHelper::UpLinkByName, "Rtr12", "Dst3");

  Simulator::Stop(Seconds(10.0));

  /****************************************************************************/
  // Tracer:

  if(isIPSimulation){
  L2RateTracer::InstallAll("inherent-drop-trace-ip.txt", Seconds(1.0));
  ndn::L3RateTracer::InstallAll("inherent-rate-trace-ip.txt", Seconds(1.0));
  ndn::AppDelayTracer::InstallAll("inherent-app-delays-trace-ip.txt");
  }
  else if(isPriorityFIFO){
  L2RateTracer::InstallAll("inherent-drop-trace-fifo.txt", Seconds(1.0));
  ndn::L3RateTracer::InstallAll("inherent-rate-trace-fifo.txt", Seconds(1.0));
  ndn::AppDelayTracer::InstallAll("inherent-app-delays-trace-fifo.txt");
  }
  else{
  L2RateTracer::InstallAll("inherent-drop-trace.txt", Seconds(1.0));
  ndn::L3RateTracer::InstallAll("inherent-rate-trace.txt", Seconds(1.0));
  ndn::AppDelayTracer::InstallAll("inherent-app-delays-trace.txt");
  }
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
