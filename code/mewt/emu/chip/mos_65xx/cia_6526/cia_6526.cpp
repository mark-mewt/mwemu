
#include "mewt/emu/chip/mos_65xx/cia_6526/cia_6526.h"
#include "mewt/diag/log.h"

namespace mewt::emu::chip::mos_65xx
{

   cia_6526_t::data_t cia_6526_t::read(address_t address)
   {
      // https://www.c64-wiki.com/wiki/CIA
      address &= 0xf;
      return *((data_t*)&_regs + address);
   }

   void cia_6526_t::write(address_t address, data_t data)
   {
      address &= 0xf;
      *((data_t*)&_regs + address) = data;
   }

}
