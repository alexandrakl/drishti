#include "drishti/ml/XGBooster.h"
#include "drishti/ml/Booster.h"
#include "drishti/ml/XGBoosterImpl.h"
#include "drishti/core/drishti_cereal_pba.h"

// include all std functions
using namespace std;
#include "xgboost/wrapper/xgboost_wrapper.h"
#include "xgboost/src/gbm/gbm.h"
#include "xgboost/src/data.h"
#include "xgboost/src/learner/learner-inl.hpp"
#include "xgboost/src/io/io.h"
#include "xgboost/src/utils/utils.h"
#include "xgboost/src/utils/math.h"
#include "xgboost/src/utils/group_data.h"
#include "xgboost/src/io/simple_dmatrix-inl.hpp"

#include <cereal/types/polymorphic.hpp>
#include <cereal/access.hpp>

using namespace xgboost;
using namespace xgboost::io;

#include <cereal/types/memory.hpp>

CEREAL_REGISTER_TYPE(xgboost::wrapper::Booster);

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(xgboost::wrapper::Booster, cereal::specialization::non_member_serialize);

DRISHTI_BEGIN_NAMESPACE(xgboost)
DRISHTI_BEGIN_NAMESPACE(wrapper)

template<class Archive>
void serialize(Archive & ar, xgboost::wrapper::Booster &m, const unsigned int version)
{
#if 0
    // CEREAL_NOTE: XGBoost requires serialization of raw pointers, which boost::serialization
    // can handle but cereal does not.
    ar & dynamic_cast<xgboost::learner::BoostLearner&>(m);
#else
    if (Archive::is_loading::value)
    {
        ar & m.model_str;
        m.LoadModelFromBuffer(&m.model_str[0], m.model_str.size());
    }
    else
    {
        bst_ulong length = 0;
        m.GetModelRaw(&length); // uses internal model_str
        ar & m.model_str;
    }
#endif
}


DRISHTI_END_NAMESPACE(wrapper)
DRISHTI_END_NAMESPACE(xgboost)

// Tree model:
typedef xgboost::tree::RTreeNodeStat RTreeNodeStat;
typedef xgboost::tree::TreeModel<bst_float, RTreeNodeStat> TreeModel;

// ##################################################################
// #################### portable_binary_*archive ####################
// ##################################################################

typedef cereal::PortableBinaryOutputArchive3 OArchive;
typedef cereal::PortableBinaryInputArchive3 IArchive;

DRISHTI_ML_NAMESPACE_BEGIN

#if !DRISHTI_BUILD_MIN_SIZE
template void XGBooster::serialize<OArchive>(OArchive &ar, const unsigned int);
template void XGBooster::Recipe::serialize<OArchive>(OArchive &ar, const unsigned int);
template void XGBooster::Impl::serialize<OArchive>(OArchive &ar, const unsigned int);
#endif

template void XGBooster::serialize<IArchive>(IArchive &ar, const unsigned int);
template void XGBooster::Recipe::serialize<IArchive>(IArchive &ar, const unsigned int);
template void XGBooster::Impl::serialize<IArchive>(IArchive &ar, const unsigned int);

DRISHTI_ML_NAMESPACE_END
