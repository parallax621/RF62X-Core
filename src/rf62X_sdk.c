#include "rf62X_sdk.h"
#include <stdarg.h>
#include "custom_string.h"
#include "platform_types.h"
#include "netwok_platform.h"
#include "memory_platform.h"

#include <mpack/mpack.h>

#ifndef _WIN32
typedef int BOOL;
typedef int SOCKET;

#define INVALID_SOCKET          (-1)
#define SOCKET_ERROR            (-1)
#define TRUE 1
#define FALSE 0
#endif

void set_platform_adapter_settings(rfUint32 host_mask, rfUint32 host_ip_addr)
{
    set_adapter_settings(host_mask, host_ip_addr);
}

rfUint8 search_scanners(vector_t *list, scanner_types_t model, rfUint32 timeout, protocol_types_t protocol)
{
    switch (model) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            rf627_old_search_by_service_protocol(
                        list, network_platform.network_settings.host_ip_addr, timeout);
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return 1; // RF627-old doesn't support this protocol
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            rf627_smart_search_by_service_protocol(
                        list, network_platform.network_settings.host_ip_addr, timeout);
            break;
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 search_scanners_by_ip(vector_t *list, scanner_types_t model, rfChar *ip, rfUint32 timeout, protocol_types_t protocol)
{
    switch (model) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            return 0;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return 1; // RF627-old doesn't support this protocol
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            rf627_smart_search_by_ip_by_service_protocol(
                        list, network_platform.network_settings.host_ip_addr, ip, timeout);
            break;
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}



hello_information get_info_about_scanner(scanner_base_t *device, protocol_types_t protocol)
{
    hello_information _hello_info = {0};
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            _hello_info.scanner_type = kRF627_OLD;
            _hello_info.protocol_type = kSERVICE;
            _hello_info.rf627old.hello_info_service_protocol = rf627_old_get_info_about_scanner_by_service_protocol(device->rf627_old);

            return _hello_info;
            break;
        }

        case kETHERNET_IP:
        case kMODBUS_TCP:
        {
            return _hello_info; // RF627-old doesn't support this protocol
            break;
        }

        default:
        {
            return _hello_info; // RF627-old doesn't support this protocol
            break;
        }
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            _hello_info.scanner_type = kRF627_SMART;
            _hello_info.protocol_type = kSERVICE;
            _hello_info.rf627smart.hello_info_service_protocol =
                    rf627_smart_get_scanner_info_by_service_protocol(device->rf627_smart);

            return _hello_info;
            break;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
        {
            return _hello_info; // RF627-old doesn't support this protocol
            break;
        }
        }
        break;
    default:
    {
        return _hello_info; // RF627-old doesn't support this protocol
        break;
    }
    }
    return _hello_info; // RF627-old doesn't support this protocol
}


rfUint8 connect_to_scanner(scanner_base_t *device, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            rfInt32 times = 3;
            for (rfInt32 i = 0; i < times; i++)
                if (rf627_old_connect(device->rf627_old))
                {
                    result = TRUE;
                    break;
                }
                else rf627_old_disconnect(device->rf627_old);
            return result;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return 1; // RF627-old doesn't support this protocol
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            rfInt32 times = 3;
            for (rfInt32 i = 0; i < times; i++)
                if (rf627_smart_connect(device->rf627_smart))
                {
                    result = TRUE;
                    break;
                }
                else rf627_smart_disconnect(device->rf627_smart);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 disconnect_from_scanner(scanner_base_t *device, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            rf627_old_disconnect(device->rf627_old);
            return TRUE;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            rf627_smart_disconnect(device->rf627_smart);
            return TRUE;
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

extern uint8_t ip_string_to_uint32 (const char*  ip_string, uint32_t* ip_addr);

rf627_profile2D_t* get_profile2D_from_scanner(
        scanner_base_t *device, rfBool zero_points, rfBool realtime, protocol_types_t protocol)
{
    rf627_profile2D_t* profile =
            memory_platform.rf_calloc(1 ,sizeof(rf627_profile2D_t));
    switch (device->type) {
    case kRF627_OLD:
        profile->type = kRF627_OLD;
        switch (protocol) {
        case kSERVICE:
            if (realtime)
            {
                if (((scanner_base_t*)device)->rf627_old->m_data_sock != NULL)
                {
                    if ((SOCKET)((scanner_base_t*)device)->rf627_old->m_data_sock != INVALID_SOCKET)
                    {
                        network_platform.network_methods.close_socket(
                                    ((scanner_base_t*)device)->rf627_old->m_data_sock);
                    }
                    ((scanner_base_t*)device)->rf627_old->m_data_sock = NULL;
                }

                rfUint32 recv_ip_addr;
                rfUint16 recv_port;
                rfInt nret;


                ((scanner_base_t*)device)->rf627_old->m_data_sock =
                        network_platform.network_methods.create_udp_socket();
                if (((scanner_base_t*)device)->rf627_old->m_data_sock != (void*)INVALID_SOCKET)
                {
                    nret = 1;
                    network_platform.network_methods.set_reuseaddr_socket_option(
                                ((scanner_base_t*)device)->rf627_old->m_data_sock);

                    network_platform.network_methods.set_socket_recv_timeout(
                                ((scanner_base_t*)device)->rf627_old->m_data_sock, STREAM_SOCK_RECV_TIMEOUT);
                    //recv_addr.sin_family = RF_AF_INET;
                    recv_port = ((scanner_base_t*)device)->rf627_old->info_by_service_protocol.profile_port;

                    //recv_addr.sin_addr = RF_INADDR_ANY;
                    recv_ip_addr = ((scanner_base_t*)device)->rf627_old->host_ip;

                    nret = network_platform.network_methods.socket_bind(
                                ((scanner_base_t*)device)->rf627_old->m_data_sock, recv_ip_addr, recv_port);
                    if (nret == RF_SOCKET_ERROR)
                    {
                        network_platform.network_methods.close_socket(((scanner_base_t*)device)->rf627_old->m_data_sock);
                        ((scanner_base_t*)device)->rf627_old->m_data_sock = NULL;
                        return NULL;
                    }
                }
            }
            profile->rf627old_profile2D = rf627_old_get_profile2D(device->rf627_old, zero_points);
            return profile;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return NULL; // RF627-old doesn't support this protocol
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        profile->type = kRF627_SMART;
        switch (protocol) {
        case kSERVICE:
            if (realtime)
            {
                if (((scanner_base_t*)device)->rf627_smart->m_data_sock != NULL)
                {
                    if ((SOCKET)((scanner_base_t*)device)->rf627_smart->m_data_sock != INVALID_SOCKET)
                    {
                        network_platform.network_methods.close_socket(
                                    ((scanner_base_t*)device)->rf627_smart->m_data_sock);
                    }
                    ((scanner_base_t*)device)->rf627_smart->m_data_sock = NULL;
                }

                rfUint32 recv_ip_addr;
                rfUint16 recv_port;
                rfInt nret;


                ((scanner_base_t*)device)->rf627_smart->m_data_sock =
                        network_platform.network_methods.create_udp_socket();
                if (((scanner_base_t*)device)->rf627_smart->m_data_sock != (void*)INVALID_SOCKET)
                {
                    nret = 1;
                    network_platform.network_methods.set_reuseaddr_socket_option(
                                ((scanner_base_t*)device)->rf627_smart->m_data_sock);

                    network_platform.network_methods.set_socket_recv_timeout(
                                ((scanner_base_t*)device)->rf627_smart->m_data_sock, STREAM_SOCK_RECV_TIMEOUT);
                    //recv_addr.sin_family = RF_AF_INET;
                    recv_port = ((scanner_base_t*)device)->rf627_smart->info_by_service_protocol.user_network_hostPort;

                    //recv_addr.sin_addr = RF_INADDR_ANY;
                    ip_string_to_uint32(((scanner_base_t*)device)->rf627_smart->
                                        info_by_service_protocol.user_network_hostIP, &recv_ip_addr);
                    recv_ip_addr = 0;

                    nret = network_platform.network_methods.socket_bind(
                                ((scanner_base_t*)device)->rf627_smart->m_data_sock, recv_ip_addr, recv_port);
                    if (nret == RF_SOCKET_ERROR)
                    {
                        network_platform.network_methods.close_socket(((scanner_base_t*)device)->rf627_smart->m_data_sock);
                        ((scanner_base_t*)device)->rf627_smart->m_data_sock = NULL;
                        return profile;
                    }
                }
            }
            profile->rf627smart_profile2D = rf627_smart_get_profile2D(device->rf627_smart, zero_points);
            return profile;
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    default:
        return NULL; // Unknown device type
        break;
    }
    return 0;
}

rf627_profile3D_t* get_profile3D_from_scanner(
        scanner_base_t *device, rfFloat step_size, rfFloat k,
        count_types_t count_type,
        rfBool zero_points,
        protocol_types_t protocol)
{
    rf627_profile3D_t* profile =
            memory_platform.rf_calloc(1 ,sizeof(rf627_profile3D_t));
    switch (device->type) {
    case kRF627_OLD:
        profile->type = kRF627_OLD;
        switch (protocol) {
        case kSERVICE:
            profile->rf627_profile3D = rf627_old_get_profile3D(
                        device->rf627_old, step_size, k, count_type, zero_points, protocol);
            return profile;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return NULL; // RF627-old doesn't support this protocol
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        profile->type = kRF627_SMART;
        switch (protocol) {
        case kSERVICE:
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    default:
        return NULL; // Unknown device type
        break;
    }
    return 0;
}

void free_smart_parameter(parameter_t* p)
{
    if (rf_strcmp(p->base.type, "uint32_t") == 0)
    {
        if(p->val_uint32->enumValues != NULL)
        {
            for(rfUint32 i = 0; i < p->val_uint32->enumValues->recCount; i++)
            {
                free(p->val_uint32->enumValues->rec[i].key);
                p->val_uint32->enumValues->rec[i].key = NULL;
                free(p->val_uint32->enumValues->rec[i].label);
                p->val_uint32->enumValues->rec[i].label = NULL;
            }
            free(p->val_uint32->enumValues->rec); p->val_uint32->enumValues->rec = NULL;
            free(p->val_uint32->enumValues); p->val_uint32->enumValues = NULL;
        }
        memory_platform.rf_free(p->val_uint32); p->val_uint32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }

        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "uint64_t") == 0)
    {
        memory_platform.rf_free(p->val_uint64); p->val_uint64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "int32_t") == 0)
    {
        memory_platform.rf_free(p->val_int32); p->val_int32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "int64_t") == 0)
    {
        memory_platform.rf_free(p->val_int64); p->val_int64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "float_t") == 0)
    {
        memory_platform.rf_free(p->val_flt); p->val_flt = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "double_t") == 0)
    {
        memory_platform.rf_free(p->val_dbl); p->val_dbl = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "u32_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_uint32->value); p->arr_uint32->value = NULL;
        memory_platform.rf_free(p->arr_uint32->defValue); p->arr_uint32->defValue = NULL;
        memory_platform.rf_free(p->arr_uint32); p->arr_uint32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "u64_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_uint64->value); p->arr_uint64->value = NULL;
        memory_platform.rf_free(p->arr_uint64->defValue); p->arr_uint64->defValue = NULL;
        memory_platform.rf_free(p->arr_uint64); p->arr_uint64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "i32_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_int32->value); p->arr_int32->value = NULL;
        memory_platform.rf_free(p->arr_int32->defValue); p->arr_int32->defValue = NULL;
        memory_platform.rf_free(p->arr_int32); p->arr_int32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "i64_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_int64->value); p->arr_int64->value = NULL;
        memory_platform.rf_free(p->arr_int64->defValue); p->arr_int64->defValue = NULL;
        memory_platform.rf_free(p->arr_int64); p->arr_int64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "flt_array_t") == 0)
    {
        memory_platform.rf_free(p->arr_flt->value); p->arr_flt->value = NULL;
        memory_platform.rf_free(p->arr_flt->defValue); p->arr_flt->defValue = NULL;
        memory_platform.rf_free(p->arr_flt); p->arr_flt = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "dbl_array_t") == 0)
    {
        memory_platform.rf_free(p->arr_dbl->value); p->arr_dbl->value = NULL;
        memory_platform.rf_free(p->arr_dbl->defValue); p->arr_dbl->defValue = NULL;
        memory_platform.rf_free(p->arr_dbl); p->arr_dbl = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "string_t") == 0)
    {
        memory_platform.rf_free(p->val_str->value); p->val_str->value = NULL;
        memory_platform.rf_free(p->val_str->defValue); p->val_str->defValue = NULL;
        memory_platform.rf_free(p->val_str); p->val_str = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL && (rf_strcmp(p->base.units, "") != 0))
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
    }
    memory_platform.rf_free(p);
    p = NULL;
}

void free_old_parameter(parameter_t* p)
{
    if (rf_strcmp(p->base.type, "uint32_t") == 0)
    {
        if(p->val_uint32->enumValues != NULL)
        {
            for(rfUint32 i = 0; i < p->val_uint32->enumValues->recCount; i++)
            {
                free(p->val_uint32->enumValues->rec[i].key);
                p->val_uint32->enumValues->rec[i].key = NULL;
                free(p->val_uint32->enumValues->rec[i].label);
                p->val_uint32->enumValues->rec[i].label = NULL;
            }
            free(p->val_uint32->enumValues->rec); p->val_uint32->enumValues->rec = NULL;
            free(p->val_uint32->enumValues); p->val_uint32->enumValues = NULL;
        }
        memory_platform.rf_free(p->val_uint32); p->val_uint32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }

        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "uint64_t") == 0)
    {
        memory_platform.rf_free(p->val_uint64); p->val_uint64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "int32_t") == 0)
    {
        memory_platform.rf_free(p->val_int32); p->val_int32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "int64_t") == 0)
    {
        memory_platform.rf_free(p->val_int64); p->val_int64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "float_t") == 0)
    {
        memory_platform.rf_free(p->val_flt); p->val_flt = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "double_t") == 0)
    {
        memory_platform.rf_free(p->val_dbl); p->val_dbl = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "u32_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_uint32->value); p->arr_uint32->value = NULL;
        memory_platform.rf_free(p->arr_uint32->defValue); p->arr_uint32->defValue = NULL;
        memory_platform.rf_free(p->arr_uint32); p->arr_uint32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "u64_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_uint64->value); p->arr_uint64->value = NULL;
        memory_platform.rf_free(p->arr_uint64->defValue); p->arr_uint64->defValue = NULL;
        memory_platform.rf_free(p->arr_uint64); p->arr_uint64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "i32_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_int32->value); p->arr_int32->value = NULL;
        memory_platform.rf_free(p->arr_int32->defValue); p->arr_int32->defValue = NULL;
        memory_platform.rf_free(p->arr_int32); p->arr_int32 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "i64_arr_t") == 0)
    {
        memory_platform.rf_free(p->arr_int64->value); p->arr_int64->value = NULL;
        memory_platform.rf_free(p->arr_int64->defValue); p->arr_int64->defValue = NULL;
        memory_platform.rf_free(p->arr_int64); p->arr_int64 = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "flt_array_t") == 0)
    {
        memory_platform.rf_free(p->arr_flt->value); p->arr_flt->value = NULL;
        memory_platform.rf_free(p->arr_flt->defValue); p->arr_flt->defValue = NULL;
        memory_platform.rf_free(p->arr_flt); p->arr_flt = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "dbl_array_t") == 0)
    {
        memory_platform.rf_free(p->arr_dbl->value); p->arr_dbl->value = NULL;
        memory_platform.rf_free(p->arr_dbl->defValue); p->arr_dbl->defValue = NULL;
        memory_platform.rf_free(p->arr_dbl); p->arr_dbl = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
        //memory_platform.rf_free(p);
    }else if (rf_strcmp(p->base.type, "string_t") == 0)
    {
        memory_platform.rf_free(p->val_str->value); p->val_str->value = NULL;
        memory_platform.rf_free(p->val_str->defValue); p->val_str->defValue = NULL;
        memory_platform.rf_free(p->val_str); p->val_str = NULL;
        memory_platform.rf_free(p->base.name); p->base.name = NULL;
        memory_platform.rf_free(p->base.access); p->base.access = NULL;
        if (p->base.units != NULL)
        {
            memory_platform.rf_free(p->base.units); p->base.units = NULL;
        }
    }
    memory_platform.rf_free(p);
    p = NULL;
//    if (rf_strcmp(p->base.type, "uint32_t") == 0)
//    {
//        if(p->val_uint32->enumValues != NULL)
//        {
//            for(rfUint32 i = 0; i < p->val_uint32->enumValues->recCount; i++)
//            {
//                free(p->val_uint32->enumValues->rec[i].key);
//                p->val_uint32->enumValues->rec[i].key = NULL;
//                free(p->val_uint32->enumValues->rec[i].label);
//                p->val_uint32->enumValues->rec[i].label = NULL;
//            }
//            free(p->val_uint32->enumValues->rec); p->val_uint32->enumValues->rec = NULL;
//            free(p->val_uint32->enumValues); p->val_uint32->enumValues = NULL;
//        }
//        memory_platform.rf_free(p->val_uint32); p->val_uint32 = NULL;
//    }else if (rf_strcmp(p->base.type, "uint64_t") == 0)
//    {
//        memory_platform.rf_free(p->val_uint64); p->val_uint64 = NULL;
//    }else if (rf_strcmp(p->base.type, "int32_t") == 0)
//    {
//        memory_platform.rf_free(p->val_int32); p->val_int32 = NULL;
//    }else if (rf_strcmp(p->base.type, "int64_t") == 0)
//    {
//        memory_platform.rf_free(p->val_int64); p->val_int64 = NULL;
//    }else if (rf_strcmp(p->base.type, "float_t") == 0)
//    {
//        memory_platform.rf_free(p->val_flt); p->val_flt = NULL;
//    }else if (rf_strcmp(p->base.type, "double_t") == 0)
//    {
//        memory_platform.rf_free(p->val_dbl); p->val_dbl = NULL;
//    }else if (rf_strcmp(p->base.type, "u32_arr_t") == 0)
//    {
//        memory_platform.rf_free(p->arr_uint32->value); p->arr_uint32->value = NULL;
//        memory_platform.rf_free(p->arr_uint32->defValue); p->arr_uint32->defValue = NULL;
//        memory_platform.rf_free(p->arr_uint32); p->arr_uint32 = NULL;
//    }else if (rf_strcmp(p->base.type, "u64_arr_t") == 0)
//    {
//        memory_platform.rf_free(p->arr_uint64->value); p->arr_uint64->value = NULL;
//        memory_platform.rf_free(p->arr_uint64->defValue); p->arr_uint64->defValue = NULL;
//        memory_platform.rf_free(p->arr_uint64); p->arr_uint64 = NULL;
//    }else if (rf_strcmp(p->base.type, "i32_arr_t") == 0)
//    {
//        memory_platform.rf_free(p->arr_int32->value); p->arr_int32->value = NULL;
//        memory_platform.rf_free(p->arr_int32->defValue); p->arr_int32->defValue = NULL;
//        memory_platform.rf_free(p->arr_int32); p->arr_int32 = NULL;
//    }else if (rf_strcmp(p->base.type, "i64_arr_t") == 0)
//    {
//        memory_platform.rf_free(p->arr_int64->value); p->arr_int64->value = NULL;
//        memory_platform.rf_free(p->arr_int64->defValue); p->arr_int64->defValue = NULL;
//        memory_platform.rf_free(p->arr_int64); p->arr_int64 = NULL;
//    }else if (rf_strcmp(p->base.type, "flt_array_t") == 0)
//    {
//        memory_platform.rf_free(p->arr_flt->value); p->arr_flt->value = NULL;
//        memory_platform.rf_free(p->arr_flt->defValue); p->arr_flt->defValue = NULL;
//        memory_platform.rf_free(p->arr_flt); p->arr_flt = NULL;
//    }else if (rf_strcmp(p->base.type, "dbl_array_t") == 0)
//    {
//        memory_platform.rf_free(p->arr_dbl->value); p->arr_dbl->value = NULL;
//        memory_platform.rf_free(p->arr_dbl->defValue); p->arr_dbl->defValue = NULL;
//        memory_platform.rf_free(p->arr_dbl); p->arr_dbl = NULL;
//    }else if (rf_strcmp(p->base.type, "string_t") == 0)
//    {
//        memory_platform.rf_free(p->val_str->value); p->val_str->value = NULL;
//        memory_platform.rf_free(p->val_str); p->val_str = NULL;
//    }
//    memory_platform.rf_free(p);
//    p = NULL;
}

rfUint8 read_params_from_scanner(scanner_base_t *device, rfUint32 timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            rfUint16 count = 0;
            rfBool ret = 0;
            while (vector_count(device->rf627_old->params_list) > 0) {
                parameter_t* p = vector_get(device->rf627_old->params_list, vector_count(device->rf627_old->params_list)-1);
                vector_delete(device->rf627_old->params_list, vector_count(device->rf627_old->params_list)-1);
                free_old_parameter(p);
                count++;
            }
            ret = rf627_old_read_user_params_from_scanner(device->rf627_old);
            ret = rf627_old_read_factory_params_from_scanner(device->rf627_old);
            return ret;
            break;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return 0; // RF627-old doesn't support this protocol
            break;
        default:
            return 0; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfUint16 count = 0;
            rfBool ret = 0;
            while (vector_count(device->rf627_smart->params_list) > 0) {
                parameter_t* p = vector_get(device->rf627_smart->params_list, vector_count(device->rf627_smart->params_list)-1);
                vector_delete(device->rf627_smart->params_list, vector_count(device->rf627_smart->params_list)-1);
                free_smart_parameter(p);
                count++;
            }
            ret = rf627_smart_read_params_from_scanner(device->rf627_smart, timeout);
            return ret;
            break;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 0; // Unknown protocol type
            break;
        }
        break;
    default:
        return 0; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 write_params_to_scanner(scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            rf627_old_write_params_to_scanner(device->rf627_old);
            return TRUE;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            rf627_smart_write_params_to_scanner(device->rf627_smart, timeout);
            return TRUE;
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 save_params_to_scanner(scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            return rf627_old_save_params_to_scanner(device->rf627_old);
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            return rf627_smart_save_params_to_scanner(device->rf627_smart, timeout);
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

parameter_t* get_parameter(scanner_base_t *device, const rfChar *param_name)
{
    switch (device->type) {
    case kRF627_OLD:
    {
        return rf627_old_get_parameter(device->rf627_old, param_name);
        break;
    }
    case kRF627_SMART:
    {
        return rf627_smart_get_parameter(device->rf627_smart, param_name);
        break;
    }
    default:
        break;
    }
    return NULL;
}

rfUint8 set_parameter(scanner_base_t *device, parameter_t* param)
{
    switch (device->type) {
    case kRF627_OLD:
    {
        return rf627_old_set_parameter(device->rf627_old, param);
        break;
    }
    case kRF627_SMART:
    {
        return rf627_smart_set_parameter(device->rf627_smart, param);
        break;
    }
    default:
        break;
    }
    return 1;
}

rfUint8 set_parameter_by_name(scanner_base_t *device, const char* param_name, rfUint32 count, va_list value)
{
    switch (device->type) {
    case kRF627_OLD:
    {
        return rf627_old_set_parameter_by_name(device->rf627_old, param_name, count, value);
        break;
    }
    case kRF627_SMART:
        break;
    default:
        break;
    }
    return 1;
}

rfUint8 send_command(
        scanner_base_t *device, command_t* command)
{
    switch (device->type) {
    case kRF627_OLD:
    {
        if(rf_strcmp("CID_PROFILE_SET_COUNTERS", command->name) == 0)
        {
            rfUint32 profile_counter = va_arg(command->arg_list, rfUint32);
            rfUint32 packet_counter = va_arg(command->arg_list, rfUint32);
            return rf627_old_command_set_counters(device->rf627_old, profile_counter, packet_counter);
        }
        break;
    }
    case kRF627_SMART:
        break;
    default:
        break;
    }
    return 1;
}


rfUint8 send_command2(
        scanner_base_t *device, command2_t* command)
{
    switch (device->type) {
    case kRF627_OLD:
    {
        if(rf_strcmp("CID_PERIPHERY_SEND", command->name) == 0)
        {
            return rf627_old_command_periphery_send(
                        device->rf627_old,
                        command->input.size, command->input.payload,
                        &command->output.size, (void**)&command->output.payload);
        }
        if(rf_strcmp("CID_PROFILE_SET_COUNTERS", command->name) == 0)
        {
            rfUint32 profile_counter = 0;
            rfUint32 packet_counter = 0;
            if (command->input.size == 4)
            {
                memcpy(&profile_counter, &command->input.payload[0], 4);
            }
            else if (command->input.size == 8)
            {
                memcpy(&profile_counter, &command->input.payload[0], 4);
                memcpy(&packet_counter, &command->input.payload[4], 4);
            }
            return rf627_old_command_set_counters(device->rf627_old, profile_counter, packet_counter);
        }
        break;
    }
    case kRF627_SMART:
        break;
    default:
        break;
    }
    return 1;
}

void free_parameter(parameter_t *param, scanner_types_t type)
{
    switch (type) {
    case kRF627_OLD:
    {
        free_old_parameter(param);
        break;
    }
    case kRF627_SMART:
        free_smart_parameter(param);
        break;
    default:
        break;
    }
}

rf627_frame_t* get_frame_from_scanner(scanner_base_t* device, protocol_types_t protocol)
{
    rf627_frame_t* frame =
            memory_platform.rf_calloc(1 ,sizeof(rf627_frame_t));
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            //char* frame = rf627_old_get_profile2D(device->rf627_old, zero_points);
            //return frame;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return NULL; // RF627-old doesn't support this protocol
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            frame->type = kRF627_SMART;
            frame->rf627smart_frame = rf627_smart_get_frame(device->rf627_smart, 200);
            return frame;
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    default:
        return NULL; // Unknown device type
        break;
    }
    return NULL;
}

rfUint8 check_connection_to_scanner(scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_old_check_connection_by_service_protocol(device->rf627_old, timeout);
            return result;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return 1; // RF627-old doesn't support this protocol
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_smart_check_connection_by_service_protocol(device->rf627_smart, timeout);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 get_dumps_profiles_from_scanner(
        scanner_base_t *device, uint32_t index, uint32_t count,
        uint32_t timeout, protocol_types_t protocol,
        rf627_profile2D_t** dump, uint32_t* dump_size, uint32_t dump_unit_size)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_smart_get_dumps_profiles_by_service_protocol(
                        device->rf627_smart, index, count, timeout,
                        dump, dump_size, dump_unit_size);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    default:
        return FALSE; // Unknown device type
        break;
    }
    return FALSE;
}

rfUint8 get_authorization_token_from_scanner(scanner_base_t *device, char **token, uint32_t* token_size, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_smart_get_authorization_token_by_service_protocol(device->rf627_smart, token, token_size, timeout);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 set_authorization_key_to_scanner(scanner_base_t *device, char *key, uint32_t key_size, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_smart_set_authorization_key_by_service_protocol(device->rf627_smart, key, key_size, timeout);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 create_calibration_table_for_scanner(
        scanner_base_t *device, uint32_t timeout)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
    {
        rfBool status = FALSE;
        status = rf627_smart_create_calibration_table(device->rf627_smart, timeout);
        return status;
    }
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 read_calibration_table_from_scanner(
        scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool status = FALSE;
            status = rf627_smart_read_calibration_table_by_service_protocol(device->rf627_smart, timeout);         
            return status;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}
rf627_calib_table_t* get_calibration_table_from_scanner(
        scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    rf627_calib_table_t* _table = calloc(1, sizeof(rf627_calib_table_t));
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            _table->type = kRF627_OLD;
            _table->rf627old_calib_table = NULL;
            return _table;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return NULL; // RF627-old doesn't support this protocol
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            _table->type = kRF627_SMART;
            timeout = 0;
            _table->rf627smart_calib_table = rf627_smart_get_calibration_table(device->rf627_smart);
            return _table;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return NULL; // Unknown protocol type
            break;
        }
        break;
    default:
        return NULL; // Unknown device type
        break;
    }
    return NULL;
}
rfUint8 set_calibration_table_to_scanner(scanner_base_t *device, rf627_calib_table_t* table, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            timeout = 0;
            result = rf627_smart_set_calibration_table(device->rf627_smart, table->rf627smart_calib_table);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 write_calibration_table_to_scanner(scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_smart_write_calibration_data_by_service_protocol(device->rf627_smart, timeout);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 save_calibration_table_to_scanner(scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
        {
            return FALSE;
        }
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
        {
            rfBool result = FALSE;
            result = rf627_smart_save_calibration_data_by_service_protocol(device->rf627_smart, timeout);
            return result;
        }
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

extern uint16_t crc16(const uint8_t *data, uint32_t len);
rfBool convert_calibration_table_to_bytes(rf627_calib_table_t* table, char** bytes, uint32_t* data_size)
{
    // Create calib_file
        mpack_writer_t writer;
        char* header = NULL;
        size_t header_size = 0;
        mpack_writer_init_growable(&writer, &header, &header_size);

        // Идентификатор сообщения для подтверждения
        mpack_start_map(&writer, 10);
        {
            mpack_write_cstr(&writer, "type");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_Type);

            mpack_write_cstr(&writer, "crc");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_CRC16);

            mpack_write_cstr(&writer, "serial");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_Serial);

            mpack_write_cstr(&writer, "data_size");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_DataSize);

            mpack_write_cstr(&writer, "data_row_length");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_DataRowLength);

            mpack_write_cstr(&writer, "width");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_Width);

            mpack_write_cstr(&writer, "height");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_Height);

            mpack_write_cstr(&writer, "mult_w");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_MultW);

            mpack_write_cstr(&writer, "mult_h");
            mpack_write_uint(&writer, table->rf627smart_calib_table->m_MultH);

            mpack_write_cstr(&writer, "time_stamp");
            mpack_write_int(&writer, table->rf627smart_calib_table->m_TimeStamp);

    //        mpack_write_cstr(&writer, "data");
    //        mpack_write_bin(&writer, (char*)table->rf627smart_calib_table->m_Data,
    //                        table->rf627smart_calib_table->m_DataSize);
        }mpack_finish_map(&writer);

        // finish writing
        if (mpack_writer_destroy(&writer) != mpack_ok) {
            fprintf(stderr, "An error occurred encoding the data!\n");
            return FALSE;
        }

        uint32_t trail_size = (header_size % 8) == 0 ? 0 : (8 - header_size % 8);
        uint32_t calib_file_size = 8 + header_size + trail_size + table->rf627smart_calib_table->m_DataSize;
        char* calib_file = calloc(calib_file_size, sizeof (char));

        uint32_t info_size = header_size;
        uint32_t data_offset = 8 + header_size + trail_size;
        memcpy(calib_file, (char*)&info_size, 4);
        memcpy(&calib_file[4], (char*)&data_offset, 4);
        memcpy(&calib_file[8], header, header_size);
        memcpy(&calib_file[8 + header_size + trail_size], table->rf627smart_calib_table->m_Data,
                table->rf627smart_calib_table->m_DataSize);

        *data_size = calib_file_size;
        *bytes = calloc(calib_file_size, sizeof (char));
        memcpy(*bytes, calib_file, calib_file_size);
        free(calib_file);
        return TRUE;

}

rf627_calib_table_t *convert_calibration_table_from_bytes(char *bytes, uint32_t data_size)
{
    mpack_tree_t tree;
    mpack_tree_init_data(&tree, (const char*)&bytes[8], data_size);
    mpack_tree_parse(&tree);
    if (mpack_tree_error(&tree) != mpack_ok)
    {
        mpack_tree_destroy(&tree);
        return NULL;
    }
    mpack_node_t root = mpack_tree_root(&tree);

    size_t header_size = tree.size;
    uint32_t trail_size = (header_size % 8) == 0 ? 0 : (8 - header_size % 8);

    rf627_calib_table_t* _table =
            (rf627_calib_table_t*)calloc(1, sizeof (rf627_calib_table_t));

    _table->type = kRF627_SMART;
    _table->rf627smart_calib_table =
            (rf627_smart_calib_table_t*)calloc(1, sizeof(rf627_smart_calib_table_t));

    _table->rf627smart_calib_table->m_Type =
            mpack_node_uint(mpack_node_map_cstr(root, "type"));

    _table->rf627smart_calib_table->m_CRC16 =
            mpack_node_uint(mpack_node_map_cstr(root, "crc"));

    _table->rf627smart_calib_table->m_Serial =
            mpack_node_uint(mpack_node_map_cstr(root, "serial"));

    _table->rf627smart_calib_table->m_DataSize =
            mpack_node_uint(mpack_node_map_cstr(root, "data_size"));

    _table->rf627smart_calib_table->m_DataRowLength =
            mpack_node_uint(mpack_node_map_cstr(root, "data_row_length"));

    _table->rf627smart_calib_table->m_Width =
            mpack_node_uint(mpack_node_map_cstr(root, "width"));

    _table->rf627smart_calib_table->m_Height =
            mpack_node_uint(mpack_node_map_cstr(root, "height"));

    _table->rf627smart_calib_table->m_MultW =
            mpack_node_float(mpack_node_map_cstr(root, "mult_w"));

    _table->rf627smart_calib_table->m_MultH =
            mpack_node_float(mpack_node_map_cstr(root, "mult_h"));

    _table->rf627smart_calib_table->m_TimeStamp =
            mpack_node_int(mpack_node_map_cstr(root, "time_stamp"));

    _table->rf627smart_calib_table->m_Data = calloc(_table->rf627smart_calib_table->m_DataSize, sizeof (char));
    memcpy(_table->rf627smart_calib_table->m_Data, &bytes[8 + header_size + trail_size], _table->rf627smart_calib_table->m_DataSize);

    return _table;
}

void free_scanner(scanner_base_t *device)
{
    switch (device->type)
    {
    case kRF627_OLD:
    {
        rf627_old_free(device->rf627_old);
        break;
    }
    case kRF627_SMART:
    {
        rf627_smart_free(device->rf627_smart);
        break;
    }
    default:
        break;
    }
}

void free_profile2D(rf627_profile2D_t *_profile)
{
    if (_profile != NULL)
    {
        switch (_profile->type) {
        case kRF627_OLD:
        {
            if(_profile->rf627old_profile2D != NULL)
            {
                free(_profile->rf627old_profile2D->intensity);
                _profile->rf627old_profile2D->intensity = NULL;
                free(_profile->rf627old_profile2D->pixels_format.pixels);
                _profile->rf627old_profile2D->pixels_format.pixels = NULL;
                free(_profile->rf627old_profile2D);
                _profile->rf627old_profile2D = NULL;
            }
            break;
        }
        case kRF627_SMART:
        {
            if(_profile->rf627smart_profile2D != NULL)
            {
                free(_profile->rf627smart_profile2D->intensity);
                _profile->rf627smart_profile2D->intensity = NULL;
                free(_profile->rf627smart_profile2D->pixels_format.pixels);
                _profile->rf627smart_profile2D->pixels_format.pixels = NULL;
                free(_profile->rf627smart_profile2D);
                _profile->rf627smart_profile2D = NULL;
            }
            break;
        }
        }
        free(_profile);
    }
}

uint8_t send_profile2D_request_to_scanner(scanner_base_t *device, rfUint32 count, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
//            rf627_old_write_params_to_scanner(device->rf627_old);
            return FALSE;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            return rf627_smart_send_profile2D_request_to_scanner(device->rf627_smart, count);
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

uint8_t send_reboot_device_request_to_scanner(scanner_base_t *device, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            return rf627_old_reboot_device_request_to_scanner(device->rf627_old);
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            return rf627_smart_reboot_device_request_to_scanner(device->rf627_smart);
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

uint8_t send_reboot_sensor_request_to_scanner(scanner_base_t *device, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
            return FALSE;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            return rf627_smart_reboot_sensor_request_to_scanner(device->rf627_smart);
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}

rfUint8 load_recovery_params_from_scanner(scanner_base_t *device, uint32_t timeout, protocol_types_t protocol)
{
    switch (device->type) {
    case kRF627_OLD:
        switch (protocol) {
        case kSERVICE:
//            rf627_old_write_params_to_scanner(device->rf627_old);
            return FALSE;
            break;
        case kETHERNET_IP:
        case kMODBUS_TCP:
            return FALSE; // RF627-old doesn't support this protocol
            break;
        default:
            return FALSE; // Unknown protocol type
            break;
        }
        break;
    case kRF627_SMART:
        switch (protocol) {
        case kSERVICE:
            return rf627_smart_load_recovery_params_from_scanner(device->rf627_smart, timeout);
            break;
        case kETHERNET_IP:
            break;
        case kMODBUS_TCP:
            break;
        default:
            return 1; // Unknown protocol type
            break;
        }
        break;
    default:
        return 2; // Unknown device type
        break;
    }
    return 0;
}



uint8_t send_data_to_scanner_periphery(
        scanner_base_t *device, const rfChar *iface_name, rfUint32 timeout,
        rfChar *in, rfUint32 in_size, rfChar **out, rfUint32 *out_size)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
        return rf627_smart_send_to_periphery_by_service_protocol(
                    device->rf627_smart, iface_name, in, in_size, out, out_size, timeout);
    }
    return FALSE;
}

uint8_t receive_data_from_scanner_periphery(
        scanner_base_t *device, const rfChar *iface_name, rfUint32 timeout,
        rfUint16 count, rfChar **out, rfUint32 *out_size)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
        return rf627_smart_receive_from_periphery_by_service_protocol(
                    device->rf627_smart, iface_name, count, out, out_size, timeout);
    }
    return FALSE;
}


uint8_t add_protocol_settings_for_cmd(
        scanner_base_t *device, const char *cmd_name,
        rfUint8 crc_enabled, rfUint8 confirm_enabled, rfUint8 one_answ,
        rfUint32 waiting_time,  rfUint32 resends_count)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
        return rf627_smart_add_protocol_settings_for_cmd(
                    device->rf627_smart, cmd_name, crc_enabled,
                    confirm_enabled, one_answ, waiting_time, resends_count);
    }
    return FALSE;
}

uint8_t send_custom_command_to_scanner(
        scanner_base_t *device, const rfChar* cmd_name, const rfChar* data_type,
        rfChar* in, rfUint32 in_size, rfChar** out, rfUint32* out_size)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
        return rf627_smart_send_custom_command(
                    device->rf627_smart, cmd_name, data_type, in, in_size, out, out_size);
    }

    return FALSE;
}

uint8_t remove_protocol_settings_for_cmd(
        scanner_base_t *device, const char *cmd_name)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
        return rf627_smart_remove_protocol_settings_for_cmd(
                    device->rf627_smart, cmd_name);
    }
    return FALSE;
}

uint8_t receive_firmware_from_scanner(scanner_base_t *device, rfUint32 timeout, rfChar **out, rfUint32 *out_size)
{
    switch (device->type) {
    case kRF627_OLD:
        return FALSE;
    case kRF627_SMART:
        return rf627_smart_receive_firmware_by_service_protocol(
                    device->rf627_smart, out, out_size, timeout);
    }
    return FALSE;
}
