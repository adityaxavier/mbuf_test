/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"


/**
 * MBUF For daily data Messaging
 */ 
#define MBUF_TEST_POOL_BUF_SIZE     (10)
#define MBUF_TEST_POOL_BUF_COUNT    (5)

#define MBUF_TEST_DATA_LEN          (100)

os_membuf_t os_mbuf_membuf[OS_MEMPOOL_SIZE(MBUF_TEST_POOL_BUF_SIZE,
        MBUF_TEST_POOL_BUF_COUNT)];

struct os_mbuf_pool ble_mbuf_pool;
struct os_mempool os_mbuf_mempool;
uint8_t os_mbuf_test_data[MBUF_TEST_DATA_LEN];

int
main(int argc, char **argv)
{
    int rc;
    rc = os_mempool_init(&os_mbuf_mempool, MBUF_TEST_POOL_BUF_COUNT,
            MBUF_TEST_POOL_BUF_SIZE, &os_mbuf_membuf[0], "mbuf_pool");
    rc = os_mbuf_pool_init(&ble_mbuf_pool, &os_mbuf_mempool,
            MBUF_TEST_POOL_BUF_SIZE, MBUF_TEST_POOL_BUF_COUNT);

    struct os_mbuf *data;
    data = os_mbuf_get_pkthdr(&ble_mbuf_pool, 13);

    sysinit();
    rc = 0;

    while (1) {

        printf("\nLength = %d\n", OS_MBUF_PKTLEN(data));

        for (int i = 0; i < OS_MBUF_PKTLEN(data); i++)
        {
            printf("%d", data->om_data[i]);
        }

        os_mbuf_append(data, "AB", 2);

        if (OS_MBUF_PKTLEN(data) > 70){

            os_mbuf_adj(data, 30);
            data = os_mbuf_trim_front(data);

            printf("Adj Length = %d\n", OS_MBUF_PKTLEN(data));
        }

        os_time_delay(OS_TICKS_PER_SEC / 4);
    }
    assert(0);

    return rc;
}

