/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include <ucd.h>
#include <assert.h>
#include "matrix_util.h"

int main()
{
    int rc;
    int input_matrix[SIZE];
    int pack_buf[SIZE];
    int unpack_buf[SIZE];
    ucd_type_t hindexed;
    int array_of_blocklengths[ROWS - 1] = {
        1, 2, 2, 4, 4, 4, 4
    };
    intptr_t array_of_displacements[ROWS - 1] = {
        9 * sizeof(int),
        18 * sizeof(int), 26 * sizeof(int),
        36 * sizeof(int), 44 * sizeof(int), 52 * sizeof(int), 60 * sizeof(int)
    };

    ucd_init(NULL);     /* before any ucd API is called the library
                         * must be initialized */

    init_matrix(input_matrix, ROWS, COLS);
    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_type_create_hindexed(ROWS - 1, array_of_blocklengths, array_of_displacements,
                                  UCD_TYPE__INT, NULL, &hindexed);
    assert(rc == UCD_SUCCESS);

    ucd_request_t request;
    uintptr_t actual_pack_bytes;
    rc = ucd_ipack(input_matrix, 1, hindexed, 0, pack_buf, 21 * sizeof(int), &actual_pack_bytes,
                   NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    uintptr_t actual_unpack_bytes;
    rc = ucd_iunpack(pack_buf, 21 * sizeof(int), unpack_buf, 1, hindexed, 0, &actual_unpack_bytes,
                     NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, ROWS, COLS, "input=");
    print_matrix(pack_buf, ROWS, COLS, "pack_buf=");
    print_matrix(unpack_buf, ROWS, COLS, "unpack_buf=");

    ucd_type_free(hindexed);
    ucd_finalize();
    return 0;
}
