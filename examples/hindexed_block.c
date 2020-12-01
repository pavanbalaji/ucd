/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include <ucd.h>
#include <assert.h>
#include <stdlib.h>
#include "matrix_util.h"

/*
 * struct pair is defined in matrix_util.h
 * each element is of type pair = (long, int)
 */

#define BLKLEN (4)

int main()
{
    int rc;
    int input_matrix[SIZE];
    int pack_buf[SIZE];
    int unpack_buf[SIZE];
    ucd_type_t hindexed_block;
    intptr_t array_of_displacements[ROWS] = {
        4 * sizeof(int), 12 * sizeof(int), 20 * sizeof(int), 28 * sizeof(int),
        32 * sizeof(int), 40 * sizeof(int), 48 * sizeof(int), 56 * sizeof(int)
    };

    ucd_init(NULL);     /* before any ucd API is called the library
                         * must be initialized */

    init_matrix(input_matrix, ROWS, COLS);
    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_type_create_hindexed_block(ROWS, BLKLEN, array_of_displacements, UCD_TYPE__INT,
                                        NULL, &hindexed_block);
    assert(rc == UCD_SUCCESS);

    ucd_request_t request;
    uintptr_t actual_pack_bytes;
    rc = ucd_ipack(input_matrix, 1, hindexed_block, 0, pack_buf, ROWS * BLKLEN * sizeof(int),
                   &actual_pack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    uintptr_t actual_unpack_bytes;
    rc = ucd_iunpack(pack_buf, ROWS * BLKLEN * sizeof(int), unpack_buf, 1, hindexed_block, 0,
                     &actual_unpack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, ROWS, COLS, "input_matrix=");
    print_matrix(unpack_buf, ROWS, COLS, "unpack_buf=");

    ucd_type_free(hindexed_block);
    ucd_finalize();
    return 0;
}
