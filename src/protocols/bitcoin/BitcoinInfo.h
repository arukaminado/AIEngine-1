/*
 * AIEngine a new generation network intrusion detection system.
 *
 * Copyright (C) 2013-2018  Luis Campo Giralte
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Ryadnology Team; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Ryadnology Team, 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 * Written by Luis Campo Giralte <me@ryadpasha.com> 
 *
 */
#ifndef SRC_PROTOCOLS_BITCOIN_BITCOININFO_H_
#define SRC_PROTOCOLS_BITCOIN_BITCOININFO_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "FlowInfo.h"

namespace aiengine {

class BitcoinInfo : public FlowInfo {
public:
        explicit BitcoinInfo() { reset(); }
        virtual ~BitcoinInfo() {}

        void reset(); 
	void serialize(JsonFlow &j);

	void incTransactions() { ++total_transactions_; }
	int32_t getTotalTransactions() const { return total_transactions_; }

	void incBlocks() { ++total_blocks_; }
	int32_t getTotalBlocks() const { return total_blocks_; }

	void incRejects() { ++total_rejects_; }
	int32_t getTotalRejects() const { return total_rejects_; }

        friend std::ostream& operator<< (std::ostream &out, const BitcoinInfo &info);

private:
	int32_t total_transactions_;
	int32_t total_blocks_;
	int32_t total_rejects_;
};

} // namespace aiengine  

#endif  // SRC_PROTOCOLS_BITCOIN_BITCOININFO_H_
