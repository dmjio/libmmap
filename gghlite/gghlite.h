#ifndef _GGHLITE_H_
#define _GGHLITE_H_

#include <stdint.h>
#include <math.h>

#include <dgsl/dgsl.h>
#include <flint/fmpz_poly.h>
#include <flint/fmpq_poly.h>
#include <flint/fmpz_mod_poly.h>
#include <flint-addons/flint-addons.h>

#include <gghlite/gghlite-defs.h>

/**
   Generate parameters for GGHLite instance requiring no randomness.

   :param self:        GGHLite public key, all fields are overwritten
   :param lambda:      security parameter `λ > 0`
   :param kappa:       multi-linearity parameter `κ > 0`
   :param rerand_mask: generate re-randomisation elements for level `i` if ``1<<(i-1) & rerand_mask``
*/

void gghlite_pk_init_params(gghlite_pk_t self, size_t lambda, size_t kappa, uint64_t rerand_mask);

/**
   Generate fields requiring randomness.

   :param self:       initialised GGHLite instance
   :param randstate:  entropy source, assumes ``flint_randinit(randstate)`` and
                      ``_flint_rand_init_gmp(randstate)`` was called
*/

void gghlite_init_instance(gghlite_t self, flint_rand_t randstate);

/**
   Initialise a new GGHLite instance.

   :param self:        GGHLite public key, all fields are overwritten
   :param lambda:      security parameter `λ > 0`
   :param kappa:       multi-linearity parameter `κ > 0`
   :param rerand_mask: generate re-randomisation elements for level `i` if ``1<<(i-1) & rerand_mask``
   :param randstate:   entropy source, assumes ``flint_randinit(randstate)`` and
                       ``_flint_rand_init_gmp(randstate)`` was called
*/

void gghlite_init(gghlite_t self, const size_t lambda, const size_t kappa, const uint64_t rerand_mask, flint_rand_t randstate);

/**
   Get a reference to the public parameters of ``op``.

   :param rop:  GGHlite public key, all fields are overwritten
   :param  op:  initialised GGHLite Instance
*/

void gghlite_pk_ref(gghlite_pk_t rop, gghlite_t op);

/**
   Clear GGHLite public key.

   :param self: all fields are cleared
*/

void gghlite_pk_clear(gghlite_pk_t self);

/**
   Clear GGHLite instance.

   The parameter ``clear_pk`` is useful to clear all secret data that was
   required to produce the public set of parameters stored in ``self->pk``.
   After ``gghlite_clear(self, 0)`` was called, it is safe to forget about
   ``self`` and to use ``self->pk`` independently (which can later be cleared by
   calling ``gghlite_pk_clear(self->pk)``. A reference to ``self->pk`` can be
   obtained by calling ``gghlite_pk_ref(·, self->pk)``.

   :param self:      all fields are cleared, except perhaps pk
   :param clear_pk:  if set, the public key is also cleared
*/

void gghlite_clear(gghlite_t self, int clear_pk);

/**
   Print sizes of parameters to STDOUT.

   :param self: assumes ``gghlite_pk_init_params(self,·,·,·)`` was called
*/

void gghlite_print_params(const gghlite_pk_t self);

/**
   Print checks to STDOUT to inspect if parameters satisfy constraints.

   :param self: assumes ``gghlite_pk_init_params(self,·,·,·)`` was called
*/

void gghlite_check_params(const gghlite_pk_t self);

/**
   Initialise encoding to zero at level 0.

   :param op:   uninitialised polynomial
   :param self: initialised GGHLite public key
*/

void gghlite_enc_init(gghlite_enc_t op, gghlite_pk_t self);

static inline void gghlite_enc_set_ui(gghlite_enc_t op, unsigned long c) {
  fmpz_mod_poly_zero(op);
  fmpz_mod_poly_set_coeff_ui(op, 0, c);
}

#define gghlite_enc_clear fmpz_mod_poly_clear

#define gghlite_clr_init  fmpz_poly_init
#define gghlite_clr_clear fmpz_poly_clear
#define gghlite_clr_equal fmpz_poly_equal

/**
   Rerandomise encoding at level `k`.

   Computes `f = f + ρ_0·b_{k,0} + ρ_1·b_{k,1}` where `ρ_i ← D_{R,σ^*}`.

   :param f:         initialised encoding at level `k`
   :param self:      initialise GGHLite public key
   :param k:         level `1 ≤ k ≤ κ`
   :param randstate: entropy source, assumes ``flint_randinit(randstate)`` and
                     ``_flint_rand_init_gmp(randstate)`` was called

   .. note::

       Note that we have no means to check that ``rop`` is indeed a level-`k'` encoding.
       If it is not the behaviour of this function is undefined.
*/

void gghlite_rerand(gghlite_enc_t rop, gghlite_pk_t self, gghlite_enc_t op, long k, flint_rand_t randstate);

/**
   Elevate an encoding at levek `k'` to level `k` and re-randomise if requested.

   :param rop:       initialised encoding at level `k'`
   :param self:      initialise GGHLite public key
   :param k:         targer level `1 ≤ k ≤ κ`
   :param kprime:    current level of ``rop`` satisfying `0 ≤ k' ≤ k`
   :param rerand:    flag controlling if re-randomisation is run after elevation
   :param randstate: entropy source, assumes ``flint_randinit(randstate)`` and
                     ``_flint_rand_init_gmp(randstate)`` was called

   .. note::

       Note that we have no means to check that ``rop`` is indeed a level-`k'` encoding.
       If it is not the behaviour of this function is undefined.
*/

void gghlite_elevate(gghlite_enc_t rop, gghlite_pk_t self, gghlite_enc_t op, long k, long kprime, int rerand, flint_rand_t randstate);

/**

   Sample a new random encoding at levek `k`.

   :param rop:       initialised encoding, return value
   :param self:      initialise GGHLite public key
   :param k:         targer level `0 ≤ k ≤ κ`
   :param randstate: entropy source, assumes ``flint_randinit(randstate)`` and
                     ``_flint_rand_init_gmp(randstate)`` was called

   .. note::

       Note that we have no means to check that ``rop`` is indeed a level-`k'` encoding.
       If it is not the behaviour of this function is undefined.

*/

void gghlite_sample(gghlite_enc_t rop, gghlite_pk_t self, long k, flint_rand_t randstate);

/**
   Encode level-0 encoding ``op`` at level `k`.

   :param rop:       initialised encoding, return value
   :param self:      initialise GGHLite public key
   :param op:        valid level-0 encoding
   :param k:         targer level `0 ≤ k ≤ κ`
   :param rerand:    rerandomise.
   :param randstate: entropy source, assumes ``flint_randinit(randstate)`` and

*/

static inline void gghlite_enc(gghlite_enc_t rop, gghlite_pk_t self, gghlite_enc_t op, long k, int rerand, flint_rand_t randstate) {
  gghlite_elevate(rop, self, op, k, 0, rerand, randstate);
}

/**
   Compute `h = f·g`.

   :param h:         initialised encoding, return value
   :param self:      initialise GGHLite public key
   :param f:         valid encoding
   :param g:         valid encoding
*/

static inline void gghlite_mult(gghlite_enc_t h, gghlite_pk_t self, gghlite_enc_t f, gghlite_enc_t g) {
  fmpz_mod_poly_mulmod(h, f, g, self->modulus);
}

/**
   Extract canonical string from ``op``

   :param rop:       initialised encoding, return value
   :param self:      initialise GGHLite public key
   :param op:        valid encoding at level-`k`
*/

void gghlite_extract(fmpz_poly_t rop, gghlite_pk_t self, gghlite_enc_t op);

#endif //_GGHLITE_H_
