/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Microsoft Corporation
 * 
 * -=- Robust Distributed System Nucleus (rDSN) -=- 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
# pragma once

# include <dsn/internal/task.h>
# include <dsn/internal/network.h>
# include <dsn/internal/synchronize.h>
# include <dsn/internal/global_config.h>

namespace dsn {

class service_node;
class rpc_engine
{
public:
    rpc_engine(configuration_ptr config, service_node* node);

    //
    // management routines
    //
    error_code start(const service_app_spec& spec);
    bool       start_server_port(int port);

    //
    // rpc registrations
    //
    bool register_rpc_handler(rpc_handler_ptr& handler);
    bool unregister_rpc_handler(task_code rpc_code) ;    

    //
    // rpc routines
    //
    void call(message_ptr& request, rpc_response_task_ptr& call);
    static void reply(message_ptr& response);
    
    //
    // information inquery
    //
    service_node* node() const { return _node; }
    const end_point& primary_address() const { return _local_primary_address; }
    void on_recv_request(message_ptr& msg, int delay_ms);

private:
    network* create_network(const network_config_spec& netcs, bool client_only);

private:
    configuration_ptr                     _config;    
    service_node                          *_node;
    std::vector<std::vector<network*>>    _client_nets; // <format, <CHANNEL, network*>>
    std::map<int, std::vector<network*>>  _server_nets; // <port, <CHANNEL, network*>>
    std::shared_ptr<rpc_client_matcher>   _matcher;
    end_point                             _local_primary_address;

    typedef std::map<std::string, rpc_handler_ptr> rpc_handlers;
    rpc_handlers                  _handlers;
    utils::rw_lock                _handlers_lock;
    
    bool                          _is_running;

    static bool                   _message_crc_required;
};

} // end namespace

