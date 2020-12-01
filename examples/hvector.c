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
    ucd_type_t hvector;

    ucd_init(NULL);     /* before any ucd API is called the library
                         * must be initialized */

    init_matrix(input_matrix, ROWS, COLS);
    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_type_create_hvector(ROWS, 1, COLS * sizeof(int), UCD_TYPE__INT, NULL, &hvector);
    assert(rc == UCD_SUCCESS);

    ucd_request_t request;
    uintptr_t actual_pack_bytes;

    rc = ucd_ipack(input_matrix, 1, hvector, 0, pack_buf, ROWS * sizeof(int), &actual_pack_bytes,
                   NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    uintptr_t actual_unpack_bytes;
    rc = ucd_iunpack(pack_buf, ROWS * sizeof(int), unpack_buf, 1, hvector, 0,
                     &actual_unpack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, ROWS, ROWS, "input_matrix=");
    print_matrix(unpack_buf, ROWS, ROWS, "unpack_buf=");

    set_matrix(unpack_buf, ROWS, COLS, 0);

    /* pack second column */
    rc = ucd_ipack(input_matrix + 1, 1, hvector, 0, pack_buf, ROWS * sizeof(int),
                   &actual_pack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    rc = ucd_iunpack(pack_buf, ROWS * sizeof(int), unpack_buf + 1, 1, hvector, 0,
                     &actual_unpack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(unpack_buf, ROWS, ROWS, "unpack_buf+1=");
    ucd_type_free(hvector);

    /* matrix transposition using hvector */
    ucd_type_t vector;

    rc = ucd_type_create_vector(ROWS, 1, COLS, UCD_TYPE__INT, NULL, &vector);
    assert(rc == UCD_SUCCESS);

    rc = ucd_type_create_hvector(COLS, 1, sizeof(int), vector, NULL, &hvector);
    assert(rc == UCD_SUCCESS);

    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_ipack(input_matrix, 1, hvector, 0, pack_buf, 256, &actual_pack_bytes, NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    rc = ucd_iunpack(pack_buf, 256, unpack_buf, 1, hvector, 0, &actual_unpack_bytes,
                     NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    fprintf(stdout, "\nMatrix transposition:\n\n");
    print_matrix(pack_buf, ROWS, COLS, "pack_buf=");
    print_matrix(unpack_buf, ROWS, ROWS, "unpack_buf=");

    ucd_type_free(vector);
    ucd_type_free(hvector);
    ucd_finalize();
    return 0;
}
