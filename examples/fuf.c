/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include <ucd.h>
#include <assert.h>
#include <stdlib.h>
#include "matrix_util.h"

#define BLKLEN (4)

int main()
{
    int rc;
    int input_matrix[SIZE];
    int pack_buf[SIZE];
    int unpack_buf[SIZE];
    ucd_type_t indexed_block;
    int array_of_displacements[ROWS] = {
        4, 12, 20, 28,
        32, 40, 48, 56
    };

    ucd_init(NULL);     /* before any ucd API is called the library
                         * must be initialized */

    init_matrix(input_matrix, ROWS, COLS);
    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_type_create_indexed_block(ROWS, BLKLEN, array_of_displacements, UCD_TYPE__INT,
                                       NULL, &indexed_block);
    assert(rc == UCD_SUCCESS);

    uintptr_t flatten_size;
    ucd_flatten_size(indexed_block, &flatten_size);

    void *flatten_type = malloc(flatten_size);
    ucd_flatten(indexed_block, flatten_type);
    ucd_type_free(indexed_block);

    ucd_type_t unflatten_type;
    rc = ucd_unflatten(&unflatten_type, flatten_type);
    assert(rc == UCD_SUCCESS);

    uintptr_t size;
    ucd_type_get_size(unflatten_type, &size);

    ucd_request_t request;
    uintptr_t actual_pack_bytes;
    rc = ucd_ipack(input_matrix, 1, unflatten_type, 0, pack_buf, ROWS * BLKLEN * sizeof(int),
                   &actual_pack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    uintptr_t actual_unpack_bytes;
    rc = ucd_iunpack(pack_buf, size, unpack_buf, 1, unflatten_type, 0, &actual_unpack_bytes,
                     NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, ROWS, COLS, "input_matrix=");
    print_matrix(unpack_buf, ROWS, COLS, "unpack_buf=");

    ucd_type_free(unflatten_type);
    free(flatten_type);
    ucd_finalize();
    return 0;
}
