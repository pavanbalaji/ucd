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

    /* create an iov of the datatype */
    uintptr_t num_iov;
    rc = ucd_iov_len(1, indexed_block, &num_iov);
    assert(rc == UCD_SUCCESS);

    fprintf(stdout, "num_iov = %lu\n", num_iov);
    struct iovec *iov_elem = malloc(sizeof(struct iovec) * num_iov);

    uintptr_t actual_iov_len;
    rc = ucd_iov((const char *) input_matrix, 1, indexed_block, 0, iov_elem, num_iov,
                 &actual_iov_len);
    assert(rc == UCD_SUCCESS);

    print_matrix(input_matrix, ROWS, COLS, "input_matrix=");

    for (uintptr_t j = 0; j < num_iov; j++) {
        fprintf(stdout, "iov_elem[%lu] => iov_len = %zu; iov_base = [ ", j,
                iov_elem[j].iov_len / sizeof(int));
        for (uintptr_t k = 0; k < iov_elem[j].iov_len / sizeof(int); k++)
            fprintf(stdout, "%.*d ", 2, ((int *) (iov_elem[j].iov_base))[k]);
        fprintf(stdout, "]\n");
    }

    free(iov_elem);

    ucd_type_free(indexed_block);
    ucd_finalize();
    return 0;
}
