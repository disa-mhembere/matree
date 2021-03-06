# Copyright 2017 Neurodata (https://neurodata.io)
# Written by Disa Mhembere (disa@cs.jhu.edu)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include ../Makefile.common

all: libstructures utils libmonya commontag examples iotag validate

libstructures:
	$(MAKE) -C structures

libmonya: libstructures utils commontag io $(OBJS)
	rm -f libmonya
	ar -cvq libmonya.a $(OBJS)

utils: io libstructures
	$(MAKE) -C utils

examples: libmonya
	$(MAKE) -C examples

validate: libmonya
	$(MAKE) -C validate

commontag:
	$(MAKE) -C common

iotag: common
	$(MAKE) -C io

clean:
	rm -f *.d
	rm -f *.o
	rm -f *~
	rm -f *.a
	$(MAKE) --ignore-errors -C structures clean
	$(MAKE) --ignore-errors -C utils clean
	$(MAKE) --ignore-errors -C examples clean
	$(MAKE) --ignore-errors -C common clean
	$(MAKE) --ignore-errors -C validate clean
	$(MAKE) --ignore-errors -C io clean

-include $(DEPS)
