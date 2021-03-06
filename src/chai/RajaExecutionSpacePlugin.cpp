// ---------------------------------------------------------------------
// Copyright (c) 2016, Lawrence Livermore National Security, LLC. All
// rights reserved.
// 
// Produced at the Lawrence Livermore National Laboratory.
// 
// This file is part of CHAI.
// 
// LLNL-CODE-705877
// 
// For details, see https:://github.com/LLNL/CHAI
// Please also see the NOTICE and LICENSE files.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// - Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
// 
// - Neither the name of the LLNS/LLNL nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
// AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
// WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// ---------------------------------------------------------------------
#include "chai/config.hpp"

#include "chai/RajaExecutionSpacePlugin.hpp"

#include "chai/ArrayManager.hpp"

namespace chai {

RajaExecutionSpacePlugin::RajaExecutionSpacePlugin() :
  m_arraymanager(chai::ArrayManager::getInstance())
{
}

void 
RajaExecutionSpacePlugin::preLaunch(RAJA::util::PluginContext p)
{
  switch (p.platform) {
    case RAJA::Platform::host: 
      m_arraymanager->setExecutionSpace(chai::CPU); break;
#if defined(CHAI_ENABLE_CUDA)
    case RAJA::Platform::cuda:
      m_arraymanager->setExecutionSpace(chai::GPU); break;
#endif
    default: 
      m_arraymanager->setExecutionSpace(chai::NONE);
  }
}

void 
RajaExecutionSpacePlugin::postLaunch(RAJA::util::PluginContext)
{
  m_arraymanager->setExecutionSpace(chai::NONE);
}

}
RAJA_INSTANTIATE_REGISTRY(RAJA::util::PluginRegistry);

// this is needed to link a dynamic lib as RAJA does not provide an exported definition of this symbol.
#if defined(_WIN32) && !defined(CHAISTATICLIB)
#ifdef CHAISHAREDDLL_EXPORTS
namespace RAJA
{
namespace util
{

PluginStrategy::PluginStrategy() = default;

}  // namespace util
}  // namespace RAJA
#endif
#endif

// Register plugin with RAJA
RAJA::util::PluginRegistry::Add<chai::RajaExecutionSpacePlugin> P(
     "RajaExecutionSpacePlugin",
     "Plugin to set CHAI execution space based on RAJA execution platform");


namespace chai {

  void linkRajaPlugin() {}

}

