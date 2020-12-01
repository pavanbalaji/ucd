/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include <ucd.h>
#include <assert.h>
#include <stdlib.h>
#include "matrix_util.h"

int main()
{
    int rc;
    int input_matrix[SIZE];
    int pack_buf[SIZE];
    int unpack_buf[SIZE];
    ucd_type_t subarray;
    int ndims = 2;
    int array_of_sizes[2] = { ROWS, COLS };
    int array_of_subsizes[2] = { 4, 4 };
    int array_of_starts[2] = { 4, 4 };
    ucd_subarray_order_e order = UCD_SUBARRAY_ORDER__C;

    ucd_init(NULL);     /* before any ucd API is called the library
                         * must be initialized */

    init_matrix(input_matrix, ROWS, COLS);
    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_type_create_subarray(ndims, array_of_sizes, array_of_subsizes,
                                  array_of_starts, order, UCD_TYPE__INT, NULL, &subarray);
    assert(rc == UCD_SUCCESS);

    ucd_request_t request;
    uintptr_t actual_pack_bytes;
    rc = ucd_ipack(input_matrix, 1, subarray, 0, pack_buf, 16 * sizeof(int), &actual_pack_bytes,
                   NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    uintptr_t actual_unpack_bytes;
    rc = ucd_iunpack(pack_buf, 16 * sizeof(int), unpack_buf, 1, subarray, 0, &actual_unpack_bytes,
                     NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, 8, 8, "input_matrix=");
    print_matrix(unpack_buf, 8, 8, "unpack_buf=");

    ucd_type_free(subarray);
    ucd_finalize();
    return 0;
}
