#ifndef _MIFE_H_
#define _MIFE_H_

#include "mife_internals.h"
#include "mife_io.h"

/* returns true iff dest is successfully initialized to the contents of src */
bool f2_matrix_copy(f2_matrix *const dest, const f2_matrix src);
bool f2_matrix_zero(f2_matrix *const dest, const unsigned int num_rows, const unsigned int num_cols);
void f2_matrix_free(f2_matrix m);
f2_matrix mife_zt_all(const mife_pp_t pp, gghlite_enc_mat_t m);

/* MIFE interface */
void mife_init_params(mife_pp_t pp, mife_flag_t flags);
void mife_mbp_set(
    void *mbp_params,
    mife_pp_t pp,
    int num_inputs,
    int (*paramfn)  (mife_pp_t, int),
    void (*kilianfn)(mife_pp_t, int *),
    void (*orderfn) (mife_pp_t, int, int *, int *),
    void (*setfn)   (mife_pp_t, mife_mat_clr_t, void *),
    int (*parsefn)  (mife_pp_t, f2_matrix)
    );
void mife_setup(mife_pp_t pp, mife_sk_t sk, int L, int lambda,
    gghlite_flag_t ggh_flags, aes_randstate_t randstate);
void mife_encrypt(mife_ciphertext_t ct, void *message, mife_pp_t pp,
    mife_sk_t sk, aes_randstate_t randstate);
int mife_evaluate(mife_pp_t pp, mife_ciphertext_t *cts);

/* memory-efficient MIFE interface */
void mife_encrypt_setup(mife_pp_t pp, fmpz_t uid, void *message,
    mife_mat_clr_t out_clr, int ****out_partitions);
void mife_encrypt_single(mife_pp_t pp, mife_sk_t sk, aes_randstate_t randstate,
    int global_index, mife_mat_clr_t clr, int ***partitions,
    gghlite_enc_mat_t out_ct);
void mife_encrypt_cleanup(mife_pp_t pp, mife_mat_clr_t clr, int ***out_partitions);

#endif /* _MIFE_H_ */
