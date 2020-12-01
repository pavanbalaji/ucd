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
    ucd_type_t vector;
    ucd_type_t vector_resized;
    ucd_type_t transpose;

    ucd_init(NULL);

    init_matrix(input_matrix, ROWS, COLS);
    set_matrix(pack_buf, ROWS, COLS, 0);
    set_matrix(unpack_buf, ROWS, COLS, 0);

    rc = ucd_type_create_vector(ROWS, 1, COLS, UCD_TYPE__INT, NULL, &vector);
    assert(rc == UCD_SUCCESS);
    rc = ucd_type_create_resized(vector, 0, sizeof(int), NULL, &vector_resized);
    assert(rc == UCD_SUCCESS);
    rc = ucd_type_create_contig(COLS, vector_resized, NULL, &transpose);
    assert(rc == UCD_SUCCESS);

    ucd_request_t request;
    uintptr_t actual_pack_bytes;
    rc = ucd_ipack(input_matrix, 1, transpose, 0, pack_buf, 256, &actual_pack_bytes,
                   NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    uintptr_t actual_unpack_bytes;
    rc = ucd_iunpack(pack_buf, 256, unpack_buf, 1, transpose, 0, &actual_unpack_bytes,
                     NULL, &request);
    assert(rc == UCD_SUCCESS);
    rc = ucd_request_wait(request);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, ROWS, COLS, "input_matrix=");
    print_matrix(pack_buf, ROWS, COLS, "pack_buf=");
    print_matrix(unpack_buf, ROWS, COLS, "unpack_buf=");

    ucd_type_free(vector);
    ucd_type_free(vector_resized);
    ucd_type_free(transpose);
    ucd_finalize();
    return 0;
}
