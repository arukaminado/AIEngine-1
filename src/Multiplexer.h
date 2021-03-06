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
#ifndef SRC_MULTIPLEXER_H_
#define SRC_MULTIPLEXER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "IPAddress.h"
#include "Packet.h"
#ifdef __FREEBSD__
#include <sys/socket.h>
#endif
#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>

namespace aiengine {

class Protocol;
typedef std::shared_ptr<Protocol> ProtocolPtr;

#define NO_PROTOCOL_SELECTED 0xFFFF

class Multiplexer;
typedef std::shared_ptr<Multiplexer> MultiplexerPtr;
typedef std::weak_ptr<Multiplexer> MultiplexerPtrWeak;

class Multiplexer {
public:
    	explicit Multiplexer();
    	virtual ~Multiplexer() {}

    	virtual void addUpMultiplexer(MultiplexerPtrWeak mux, unsigned int key) {
	
		muxUpMap_[key] = mux;
	}

	virtual void addDownMultiplexer(MultiplexerPtrWeak mux) {
	
		muxDown_ = mux;
	}

	MultiplexerPtrWeak getDownMultiplexer() const;
	MultiplexerPtrWeak getUpMultiplexer(int key) const;

	void forwardPacket(Packet& packet);

        void statistics(std::basic_ostream<char> &out);
        void statistics() { statistics(std::cout); }

	int getNumberUpMultiplexers() const { return muxUpMap_.size(); }

	void setProtocolIdentifier(uint16_t protocol_id) { protocol_id_ = protocol_id; }
	uint16_t getProtocolIdentifier() const { return protocol_id_; }
	uint16_t getNextProtocolIdentifier() const { return next_protocol_id_; }
	void setNextProtocolIdentifier(uint16_t protocol_id) { next_protocol_id_ = protocol_id; }
	void setProtocol(ProtocolPtr proto) { proto_ = proto; }
	ProtocolPtr getProtocol() { return proto_; }

	void setHeaderSize(int size) { header_size_ = size; }
	int getHeaderSize() { return header_size_; }

	void setPacketInfo(const uint8_t *packet, int length, int prev_header_size, PacketAnomalyType pa,time_t packet_time);
	void setPacket(Packet *packet);

	void addChecker(std::function <bool (Packet&)> checker) { check_func_ = checker; }
	void addPacketFunction(std::function <bool (Packet&)> packet_func) { packet_func_ = packet_func; }

	uint64_t getTotalForwardPackets() const { return total_forward_packets_; }
	uint64_t getTotalFailPackets() const { return total_fail_packets_; }
	uint64_t getTotalReceivedPackets() const { return total_received_packets_; }

	Packet *getCurrentPacket() { return &packet_;}

	bool acceptPacket(Packet &packet) const { return check_func_(packet); }

	IPAddress address;
	// This is realy uggly puagggggg
	uint16_t total_length;
private:
	ProtocolPtr proto_;
	Packet packet_;
	uint64_t total_received_packets_;
	uint64_t total_forward_packets_;
	uint64_t total_fail_packets_;
	MultiplexerPtrWeak muxDown_;
	int header_size_;
	int offset_;
	uint16_t protocol_id_; // the protocol analiyzer owned by the multiplexer
	uint16_t next_protocol_id_; // the next protocol to check by the multiplexer
    	typedef std::unordered_map<int,MultiplexerPtrWeak> MuxMap;
	MuxMap muxUpMap_;
	std::function <bool (Packet&)> check_func_;
	std::function <bool (Packet&)> packet_func_;
};

} // namespace aiengine

#endif  // SRC_MULTIPLEXER_H_
