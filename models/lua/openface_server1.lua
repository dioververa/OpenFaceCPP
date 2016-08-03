--#!/usr/bin/env th
--
-- Copyright 2015-2016 Carnegie Mellon University
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.


require 'torch'
require 'nn'
require 'dpnn'
require 'image'
require 'nnx'
require 'dok'

print("feature extraction")

io.stdout:setvbuf 'no'
torch.setdefaulttensortype('torch.FloatTensor')


function getFeatures(sampledata,netTorchPath,imgDim)
net = torch.load(netTorchPath)
net:evaluate()

	local cuda = false
	local imgCuda = nil
	if cuda then
	   require 'cutorch'
	   require 'cunn'
	   net = net:cuda()
	   imgCuda = torch.CudaTensor(1, 3, imgDim, imgDim)
	end

	local img = torch.Tensor(1, 3, imgDim, imgDim)
	img[1] = sampledata:reshape(3,imgDim,imgDim)

local rep
local m = {}

      rep = net:forward(img)

      local sz = rep:size(1)
      for i = 1,sz do
	m[i] = rep[i]
      end
	   
return m	   
end


