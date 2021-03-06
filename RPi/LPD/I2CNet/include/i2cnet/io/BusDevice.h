/**
 * Copyright 2016 Graeme Farquharson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef I2CNET_IO_BUS_H_
#define I2CNET_IO_BUS_H_

#include <string>
#include <vector>

namespace i2cnet {
namespace io {

class BusDevice {
public:
	BusDevice(const std::string& busName);
	virtual ~BusDevice();

	void read(char address, char location, int length, std::vector<char>& data);
	void write(char address, char location, const std::vector<char>& data);

	void setAddress(char address);

private:
	BusDevice(const BusDevice&);
	BusDevice const& operator=(BusDevice const&);

	int fileDescriptor_;
	std::string busName_;
};

}
}

#endif
