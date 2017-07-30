#ifndef HGAPREC_HH
#define HGAPREC_HH

#include "env.hh"
#include "ratings.hh"
#include "gpbase.hh"

class HGAPRec
{
public:
  HGAPRec(Env &env, Ratings &ratings);
  ~HGAPRec();

  void vb_hier();
  void load_beta();

private:
  void initialize();
  void approx_log_likelihood();

  void get_phi(GPBase<Matrix> &a, uint32_t ai,
               GPBase<Matrix> &b, uint32_t bi,
               Array &phi);

  void get_phi(GPBase<Matrix> &a, uint32_t ai,
               GPBase<Matrix> &b, uint32_t bi,
               double biasa, double biasb,
               Array &phi);
  void get_phi(Matrix &a, uint32_t ai,
               GPBase<Matrix> &b, uint32_t bi,
               Array &phi);

  void get_phi(GPBase<Matrix> &a, uint32_t ai,
               Matrix &b, uint32_t bi,
               Array &phi);

  void load_validation_and_test_sets();
  void compute_likelihood(bool validation);
  double pair_likelihood(uint32_t p, uint32_t q, yval_t y) const;
  double log_factorial(uint32_t n) const;

  void do_on_stop();
  void compute_precision(bool save_ranking_file);

  double prediction_score_hier(uint32_t user, uint32_t movie) const;
  void save_model();
  double rating_likelihood_hier(uint32_t p, uint32_t q, yval_t y) const;
  uint32_t duration() const;
  bool is_validation(const Rating &r) const;

  Env &_env;
  Ratings &_ratings;

  uint32_t _n;
  uint32_t _m;
  uint32_t _k;
  uint32_t _iter;

  GPMatrixGR _theta;
  GPMatrixGR _beta;

  GPMatrix _thetabias;
  GPMatrix _betabias;

  GPMatrix _htheta;
  GPMatrix _hbeta;

  GPArray _thetarate;
  GPArray _betarate;

  Matrix _theta_mle;
  Matrix _beta_mle;
  Matrix _old_theta_mle;
  Matrix _old_beta_mle;

  CountMap _validation_map;
  CountMap _test_map;
  FreqMap _validation_users_of_movie;
  IDMap _leave_one_out;

  IDMap _ctr_user_to_idx;
  IDMap _ctr_item_to_idx;

  UserMap _sampled_users;
  UserMap _sampled_movies;

  Matrix *_nmf_theta;
  Matrix *_nmf_beta;
  Matrix *_ctr_theta;
  Matrix *_ctr_beta;
  Matrix *_lda_gamma;
  Matrix *_lda_beta;
  Matrix *_chi_gamma;
  Matrix *_chi_beta;
  Matrix *_chi_ubias;
  Matrix *_chi_vbias;
  Matrix *_chi_global;

  uint32_t _start_time;
  gsl_rng *_r;

  FILE *_hf;
  FILE *_vf;
  FILE *_tf;
  FILE *_af;
  FILE *_pf;
  FILE *_df;
  FILE *_rf;

  double _prev_h;
  uint32_t _nh;
  bool _save_ranking_file;
  bool _use_rate_as_score;
  uint32_t _topN_by_user;
  uint32_t _maxval, _minval;

  bool _mle_user;
  bool _mle_item;
};

inline uint32_t
HGAPRec::duration() const
{
  time_t t = time(0);
  return t - _start_time;
}

inline bool
HGAPRec::is_validation(const Rating &r) const
{
  assert(r.first < _n && r.second < _m);
  CountMap::const_iterator itr = _validation_map.find(r);
  if (itr != _validation_map.end())
    return true;
  return false;
}

#endif
