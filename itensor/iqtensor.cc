//
// Distributed under the ITensor Library License, Version 1.0.
//    (See accompanying LICENSE file.)
//
#include "iqtensor.h"
#include "qcounter.h"
#include <set>
using namespace std;
using boost::format;
using boost::array;
using boost::shared_ptr;
using boost::make_shared;


IQTDat::
IQTDat() 
    :
    rmap_init(false)
    { }

IQTDat::
IQTDat(const IQTDat& other) 
    : 
    itensor(other.itensor), 
    rmap_init(false)
	{ }

IQTDat::
IQTDat(istream& s) 
    {
    read(s);
    }


void IQTDat::
read(istream& s)
    { 
    rmap_init = false;
	size_t size;
	s.read((char*) &size,sizeof(size));
	itensor.resize(size);
    Foreach(ITensor& t, itensor)
        { 
        t.read(s); 
        }
    }

void IQTDat::
write(ostream& s) const
	{
	size_t size = itensor.size();
	s.write((char*) &size,sizeof(size));
    Foreach(const ITensor& t, itensor)
        { t.write(s); }
	}

const shared_ptr<IQTDat>& IQTDat::
Null()
    {
    static shared_ptr<IQTDat> Null_ = make_shared<IQTDat>();
    return Null_;
    }

void IQTDat::
init_rmap() const
	{
	if(rmap_init) return;

    for(iterator it = itensor.begin(); it != itensor.end(); ++it)
	    rmap[ApproxReal(it->uniqueReal())] = it;

	rmap_init = true;
	}

void IQTDat::
uninit_rmap() const 
	{ 
	rmap.clear();
	rmap_init = false; 
	}

bool IQTDat::
has_itensor(const ApproxReal& r) const
	{ 
	init_rmap();
	return rmap.count(r) == 1; 
	}

void IQTDat::
clear()
    {
    uninit_rmap();
    itensor.clear();
    }

void IQTDat::
insert(const ApproxReal& r, const ITensor& t)
    {
    init_rmap();
    if(rmap.count(r) == 1)
        {
        Print((*rmap[r])); 
        Print(t);
        Error("Can't insert ITensor with identical structure twice, use operator+=.");
        }
    else
        {
        itensor.push_front(t);
        rmap[r] = itensor.begin();
        }
    }

void IQTDat::
insert(const ITensor& t)
    {
    ApproxReal r(t.uniqueReal());
    insert(r,t);
    }

void IQTDat::
insert_add(const ApproxReal& r, const ITensor& t)
    {
    init_rmap();
    if(rmap.count(r) == 1)
        {
        *rmap[r] += t;
        return;
        }
    else
        {
        itensor.push_front(t);
        rmap[r] = itensor.begin();
        }
    }

void IQTDat::
insert_add(const ITensor& t)
    {
    ApproxReal r(t.uniqueReal());
    insert_add(r,t);
    }

void IQTDat::
clean(Real min_norm)
    {
    list<ITensor> nitensor;
    Foreach(const ITensor& t, itensor)
        {
        if(t.norm() >= min_norm)
            nitensor.push_back(t);
        }
    swap(nitensor);
    }

void IQTDat::
swap(StorageT& new_itensor)
    {
    uninit_rmap();
    itensor.swap(new_itensor);
    }

void IQTDat::
scaleTo(const LogNumber& newscale)
    {
    Foreach(ITensor& t, itensor)
        t.scaleTo(newscale);
    }

//
// IQTensor
//

bool IQTensor::
isNull() const { return p == IQTDat::Null(); }

bool IQTensor::
isNotNull() const { return p != IQTDat::Null(); }

int IQTensor::
r() const 
    { 
    return is_->r(); 
    }

const IQIndex& IQTensor::
index(int j) const 
    { 
    if(!is_) Error("IQTensor is null");
    return is_->index(j); 
    }

int IQTensor::
iten_size() const { return p->size(); }

bool IQTensor::
iten_empty() const { return p->empty(); }

//----------------------------------------------------
//IQTensor: Constructors 

IQTensor::
IQTensor() 
    : 
    is_(IndexSet<IQIndex>::Null()),
    p(IQTDat::Null()) 
    { }

IQTensor::
IQTensor(Real val) 
    : 
    is_(make_shared<IndexSet<IQIndex> >()),
    p(make_shared<IQTDat>())
    { 
    operator+=(ITensor(val));
    }

IQTensor::
IQTensor(const IQIndex& i1) 
    : 
    is_(make_shared<IndexSet<IQIndex> >(i1)),
    p(make_shared<IQTDat>()) 
    { 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2) 
    : 
    is_(make_shared<IndexSet<IQIndex> >(i1,i2)),
    p(make_shared<IQTDat>()) 
    { 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2,const IQIndex& i3) 
	: 
    is_(make_shared<IndexSet<IQIndex> >(i1,i2,i3)),
    p(make_shared<IQTDat>()) 
    { 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2,const IQIndex& i3,
         const IQIndex& i4) 
    : 
    is_(make_shared<IndexSet<IQIndex> >(i1,i2,i3,i4)),
    p(make_shared<IQTDat>())
    { 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2,const IQIndex& i3,
         const IQIndex& i4,const IQIndex& i5) 
    : 
    is_(make_shared<IndexSet<IQIndex> >(i1,i2,i3,i4,i5)),
    p(make_shared<IQTDat>()) 
    { 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2,const IQIndex& i3,
         const IQIndex& i4,const IQIndex& i5,const IQIndex& i6)
	: 
    is_(make_shared<IndexSet<IQIndex> >(i1,i2,i3,i4,i5,i6)),
    p(make_shared<IQTDat>()) 
	{ 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2,const IQIndex& i3,
         const IQIndex& i4,const IQIndex& i5,const IQIndex& i6,
         const IQIndex& i7)
	: 
    is_(make_shared<IndexSet<IQIndex> >(i1,i2,i3,i4,i5,i6,i7)),
    p(make_shared<IQTDat>()) 
	{ 
    }

IQTensor::
IQTensor(const IQIndex& i1,const IQIndex& i2,const IQIndex& i3,
         const IQIndex& i4,const IQIndex& i5,const IQIndex& i6,
         const IQIndex& i7,const IQIndex& i8)
	: 
    is_(new IndexSet<IQIndex>(i1,i2,i3,i4,i5,i6,i7,i8)),
    p(make_shared<IQTDat>()) 
	{ 
    }

IQTensor::
IQTensor(std::vector<IQIndex>& iqinds_) 
	: 
    is_(new IndexSet<IQIndex>(iqinds_)),
    p(make_shared<IQTDat>()) 
	{ 
#ifdef DEBUG
    Foreach(const IQIndex& I, iqinds_)
        {
        if(I == IQIndex::Null())
            Error("IQIndex is null");
        }
#endif
    }

IQTensor::
IQTensor(const IQIndexVal& iv1) 
    : 
    is_(make_shared<IndexSet<IQIndex> >(iv1.iqind)),
    p(make_shared<IQTDat>()) 
	{ 
	operator()(iv1) = 1;
	}

IQTensor::
IQTensor(const IQIndexVal& iv1, const IQIndexVal& iv2) 
	: 
    is_(make_shared<IndexSet<IQIndex> >(iv1.iqind,iv2.iqind)),
    p(make_shared<IQTDat>()) 
	{ 
    operator()(iv1,iv2) = 1;
	}

IQTensor::
IQTensor(const IQIndexVal& iv1, const IQIndexVal& iv2,
         const IQIndexVal& iv3)
	: 
    is_(make_shared<IndexSet<IQIndex> >(iv1.iqind,iv2.iqind,iv3.iqind)),
    p(make_shared<IQTDat>()) 
	{ 
    operator()(iv1,iv2,iv3) = 1;
	}

IQTensor::
IQTensor(const IQIndexVal& iv1, const IQIndexVal& iv2,
         const IQIndexVal& iv3, const IQIndexVal& iv4)
	: 
    is_(make_shared<IndexSet<IQIndex> >(iv1.iqind,iv2.iqind,iv3.iqind,iv4.iqind)),
    p(make_shared<IQTDat>()) 
	{ 
    operator()(iv1,iv2,iv3,iv4) = 1;
	}

IQTensor::
IQTensor(IndexType type,const IQTensor& other) 
    : 
    is_(other.is_),
    p(other.p)
    { prime(type); }

IQTensor::
IQTensor(std::istream& s)
    { 
    read(s); 
    }

const IQTensor& IQTensor::
Complex_1()
    {
    static const IQTensor Complex_1_(IQIndex::IndReIm()(1));
    return Complex_1_;
    }

const IQTensor& IQTensor::
Complex_i()
    {
    static const IQTensor Complex_i_(IQIndex::IndReIm()(2));
    return Complex_i_;
    }

IQTensor
IQmakeComplexProd()
    {
    IQTensor iqprod(IQIndex::IndReIm(),
                    IQIndex::IndReImP(),
                    IQIndex::IndReImPP());

    iqprod += ITensor::ComplexProd();
    return iqprod;
    }

const IQTensor& IQTensor::
ComplexProd()
    {
    static const IQTensor ComplexProd_(IQmakeComplexProd());
    return ComplexProd_;
    }

void IQTensor::
read(std::istream& s)
    {
    bool null_;
    s.read((char*) &null_,sizeof(null_));
    if(null_) 
        { *this = IQTensor(); return; }
    is_ = make_shared<IndexSet<IQIndex> >();
    is_->read(s);
    p = make_shared<IQTDat>();
    p->read(s);
    }

void IQTensor::
write(std::ostream& s) const
	{
	bool null_ = isNull();
	s.write((char*) &null_,sizeof(null_));
	if(null_) return;
    is_->write(s);
	p->write(s);
	}

IQTensor& IQTensor::
operator*=(Real fac) 
    { 
    soloDat();

    if(fac == 0) 
        { 
        p->clear(); 
        return *this; 
        }

    Foreach(ITensor& t, *p)
        {
        t *= fac;
        }

    return *this; 
    }

IQTensor& IQTensor::
operator/=(Real fac) 
    { 
    soloDat();

    if(fac == 0) 
        { 
        p->clear(); 
        return *this; 
        }

    Foreach(ITensor& t, *p)
        {
        t /= fac;
        }

    return *this; 
    }

IQTensor& IQTensor::
operator*=(const LogNumber& lgnum) 
    { 
    soloDat();

    Foreach(ITensor& t, *p)
        {
        t *= lgnum;
        }

    return *this; 
    }

void IQTensor::
insert(const ITensor& t) 
    { 
    if(t.scale().sign() != 0)
        {
        soloDat();
        p->insert(t);
        }
    }

IQTensor& IQTensor::
operator+=(const ITensor& t) 
    { 
    if(t.scale().sign() != 0)
        {
        soloDat();
        p->insert_add(t);
        }
    return *this;
    }

//Non-const element access
Real& IQTensor::
operator()(const IQIndexVal& iv1, const IQIndexVal& iv2,
           const IQIndexVal& iv3, const IQIndexVal& iv4, 
           const IQIndexVal& iv5, const IQIndexVal& iv6,
           const IQIndexVal& iv7, const IQIndexVal& iv8)
	{
    soloDat();
    boost::array<IQIndexVal,NMAX+1> iv 
        = {{ IQIndexVal::Null(), iv1, iv2, iv3, iv4, iv5, iv6, iv7, iv8 }};

    Real ur = 0; 
    int nn = 0; 
    while(GET(iv,nn+1).iqind != IQIndexVal::Null().iqind) 
        ur += GET(iv,++nn).index().uniqueReal(); 
    if(nn != r()) 
        Error("Wrong number of IQIndexVals provided");
    ApproxReal r(ur);

    if(!p->has_itensor(r))
        {
        std::vector<Index> indices; 
        indices.reserve(nn);
        for(int j = 1; j <= nn; ++j) 
            {
            if(!hasindex(iv[j].iqind)) 
                Error("IQTensor::operator(): IQIndex not found.");
            indices.push_back(iv[j].index());
            }
        ITensor t(indices);
        p->insert_add(r,t);
        }

    return (p->get(r)).operator()(iv1.blockIndexVal(),
                                       iv2.blockIndexVal(),
                                       iv3.blockIndexVal(),
                                       iv4.blockIndexVal(),
                                       iv5.blockIndexVal(),
                                       iv6.blockIndexVal(),
                                       iv7.blockIndexVal(),
                                       iv8.blockIndexVal());
	}

//const element access
Real IQTensor::
operator()(const IQIndexVal& iv1, const IQIndexVal& iv2,
           const IQIndexVal& iv3, const IQIndexVal& iv4, 
           const IQIndexVal& iv5, const IQIndexVal& iv6,
           const IQIndexVal& iv7, const IQIndexVal& iv8) const
	{
    boost::array<IQIndexVal,NMAX+1> iv 
        = {{ IQIndexVal::Null(), iv1, iv2, iv3, iv4, iv5, iv6, iv7, iv8 }};

    Real ur = 0; 
    int nn = 0; 
    while(GET(iv,nn+1).iqind != IQIndexVal::Null().iqind) 
        ur += GET(iv,++nn).index().uniqueReal(); 
    if(nn != r()) 
        Error("Wrong number of IQIndexVals provided");
    ApproxReal r(ur);

    if(!p->has_itensor(r))
        {
        return 0.;
        }
    else
        {
        return (p->get(r)).operator()(iv1.blockIndexVal(),
                                         iv2.blockIndexVal(),
                                         iv3.blockIndexVal(),
                                         iv4.blockIndexVal(),
                                         iv5.blockIndexVal(),
                                         iv6.blockIndexVal(),
                                         iv7.blockIndexVal(),
                                         iv8.blockIndexVal());
        }
	}

//Method for specifically requesting const access
Real IQTensor::
at(const IQIndexVal& iv1, const IQIndexVal& iv2,
   const IQIndexVal& iv3, const IQIndexVal& iv4, 
   const IQIndexVal& iv5, const IQIndexVal& iv6,
   const IQIndexVal& iv7, const IQIndexVal& iv8) const
    {
    return operator()(iv1,iv2,iv3,iv4,iv5,iv6,iv7,iv8);
    }


QN IQTensor::
div() const
	{
	QN div_;
	assert(p != 0);
	if(p->empty())
	    {   
	    this->printIndices("this");
	    Error("IQTensor has no blocks");
	    }
	const ITensor& t = *(p->begin());
	for(int j = 1; j <= t.r(); ++j)
	    div_ += qn(t.index(j))*dir(t.index(j));
	return div_;
	}


QN IQTensor::
qn(const Index& in) const
	{
	int iqq = find_iqind(in);
	if(iqq == 0) 
	    Error("qn: cant find index");
	return is_->index(iqq).qn(in);
	} 

Arrow IQTensor::
dir(const Index& in) const
	{
	int iqq = find_iqind(in);
	if(iqq == 0) 
	    {
	    this->print("this IQTensor");
	    in.print("in"); 
	    Error("IQTensor::dir(Index&): cant find Index in IQIndices");
	    }
	return is_->index(iqq).dir();
	}


void IQTensor::
noprime(IndexType type)
	{
	solo();

    is_->noprime(type);

    Foreach(ITensor& t, *p)
        t.noprime(type); 
	} 

void IQTensor::
prime(IndexType type, int inc)
	{
	solo();

    is_->prime(type,inc);

    Foreach(ITensor& t, *p)
	    t.prime(type,inc);
	}

void IQTensor::
mapprime(int plevold, int plevnew, IndexType type)
    {
    solo();

    is_->mapprime(plevold,plevnew,type);

    Foreach(ITensor& t, *p)
	    t.mapprime(plevold,plevnew,type);
	}

void IQTensor::
prime(const IQIndex& I, int inc)
	{
	solo();

    is_->prime(I,inc);

    Foreach(ITensor& t, *p)
    for(std::vector<inqn>::const_iterator
        x = I.iq().begin(); x != I.iq().end(); ++x)
        {
		if(t.hasindex(x->index)) 
		    t.prime(x->index,inc);
        }
	}

void IQTensor::
noprime(const IQIndex& I)
	{
	solo();

    is_->noprime(I);

    Foreach(ITensor& t, *p)
    for(std::vector<inqn>::const_iterator
        x = I.iq().begin(); x != I.iq().end(); ++x)
        {
        if(t.hasindex(x->index)) 
            t.noprime(x->index);
        }
	}


int IQTensor::
find_iqind(const Index& ii) const
    {
    for(int j = 1; j <= is_->r(); ++j)
        {
        if(is_->index(j).hasindex(ii)) 
            return j;
        }
    return 0;
    }

bool IQTensor::
uses_ind(const Index& ii) const
    {
    Foreach(const ITensor& t, *p)
        {
        if(t.hasindex(ii)) 
            return true;
        }
    return false;
    }

bool IQTensor::
hastype(IndexType t) const 
    { 
    if(!is_) return false;
    return is_->hastype(t); 
    }

const IQIndex& IQTensor::
findtype(IndexType t) const 
    { 
    if(!is_) Error("findtype failed, IQTensor is null");
    return is_->findtype(t); 
    }

const IQIndex& IQTensor::
finddir(Arrow dir) const 
    { 
    if(!is_) Error("finddir failed, IQTensor is null");
    return is_->finddir(dir); 
    }

bool IQTensor::
hasindex(const IQIndex& I) const 
    { 
    if(!is_) return false;
    return is_->hasindex(I); 
    }

Real IQTensor::
uniqueReal() const 
    { 
    if(!is_) Error("IQTensor is null");
    return is_->uniqueReal(); 
    }

Real IQTensor::
norm() const
    {
    Real res = 0;
    Foreach(const ITensor& t, *p)
        { res += sqr(t.norm()); }

    //Even if the ITensor norms aren't separately too
    //large for Real, their sum may be
    if(res > std::numeric_limits<Real>::max())
        {
        throw TooBigForReal("Norm too large for real in IQTensor::norm()");
        }

    return sqrt(res);
    }

Real IQTensor::
sumels() const
    {
    Real res = 0;
    Foreach(const ITensor& t, *p)
        { res += t.sumels(); }
    return res;
    }

void IQTensor::
scaleOutNorm()
    {
    Real f = norm();
    Foreach(ITensor& t, *p)
        t.scaleTo(f);
    }

void IQTensor::
scaleTo(const LogNumber& newscale)
    {
    Foreach(ITensor& t, *p)
        t.scaleTo(newscale);
    }

void IQTensor::
clean(Real min_norm)
    { 
    soloDat(); 
    p->clean(min_norm); 
    }

void IQTensor::
addindex1(const IQIndex& I)
	{
	if(I.m() != 1) 
	    Error("IQTensor::operator*=(IQIndex): IQIndex must have m == 1.");    
	solo(); 
    Foreach(ITensor& t, *p)
        { t.addindex1(I.index(1)); }
	is_->addindex(I);
	}

void IQTensor::
tieIndices(const boost::array<IQIndex,NMAX>& indices, int niqind, 
           const IQIndex& tied)
    {
    if(niqind < 1) Error("No IQIndices to tie");

    const int nindex = indices[0].nindex();

    shared_ptr<IQTDat> np = make_shared<IQTDat>();
    shared_ptr<IndexSet<IQIndex> > nis_ = make_shared<IndexSet<IQIndex> >(tied);

    int nmatched = 0;
    for(int k = 1; k <= is_->r(); ++k)
        {
        const IQIndex& K = is_->index(k);
        bool K_is_tied = false;
        for(int j = 0; j < niqind; ++j)
        if(K == indices[j]) 
            {
            if(indices[j].m() != tied.m())
                Error("Tied indices must have matching m's");
            K_is_tied = true;
            ++nmatched;
            break;
            }
        if(!K_is_tied)
            {
            nis_->addindex(K);
            }
        }

    if(nmatched != niqind)
        {
        PrintIndices((*this));
        cout << "Indices to tie = " << endl;
        for(int j = 0; j < niqind; ++j)
            cout << indices[j] << endl;
        Error("Couldn't find IQIndex to tie");
        }

    array<Index,NMAX> totie;
    for(int i = 1; i <= nindex; ++i)
        {
        for(int n = 0; n < niqind; ++n)
            totie[n] = indices[n].index(i);

        Foreach(const ITensor& t, *p)
            {
            if(!t.hasAllIndex(totie,niqind)) continue;
            ITensor nt(t);
            nt.tieIndices(totie,niqind,tied.index(i));
            np->insert_add(nt);
            }
        }
    p.swap(np);
    is_.swap(nis_);
    }

void IQTensor::
tieIndices(const IQIndex& i1, const IQIndex& i2, const IQIndex& tied)
    {
    boost::array<IQIndex,NMAX> inds =
        {{ i1, i2, 
           IQIndex::Null(), IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null() }};

    tieIndices(inds,2,tied);
    }

void IQTensor::
trace(const boost::array<IQIndex,NMAX>& indices, int niqind)
    {
    if(niqind < 1) Error("No IQIndices to trace");

    const int nindex = indices[0].nindex();
    const int tm = indices[0].m();

    shared_ptr<IQTDat> np = make_shared<IQTDat>();
    shared_ptr<IndexSet<IQIndex> > nis_ = make_shared<IndexSet<IQIndex> >();

    int nmatched = 0;
    for(int k = 1; k <= is_->r(); ++k)
        {
        const IQIndex& K = is_->index(k);
        bool K_traced = false;
        for(int j = 0; j < niqind; ++j)
        if(K == indices[j]) 
            {
            if(indices[j].m() != tm)
                Error("Traced indices must have matching m's");
            K_traced = true;
            ++nmatched;
            break;
            }
        if(!K_traced)
            {
            nis_->addindex(K);
            }
        }

    if(nmatched != niqind)
        {
        PrintIndices((*this));
        cout << "Indices to trace = " << endl;
        for(int j = 0; j < niqind; ++j)
            cout << indices[j] << endl;
        Error("Couldn't find IQIndex to trace");
        }

    array<Index,NMAX> totrace;
    for(int i = 1; i <= nindex; ++i)
        {
        for(int n = 0; n < niqind; ++n)
            totrace[n] = indices[n].index(i);

        Foreach(const ITensor& t, *p)
            {
            if(!t.hasAllIndex(totrace,niqind)) continue;
            ITensor tt(t);
            tt.trace(totrace,niqind);
            np->insert_add(tt);
            }
        }
    p.swap(np);
    is_.swap(nis_);
    }

void IQTensor::
trace(const IQIndex& i1, const IQIndex& i2)
    {
    boost::array<IQIndex,NMAX> inds =
        {{ i1, i2, 
           IQIndex::Null(), IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null() }};

    trace(inds,2);
    }

void IQTensor::
trace(const IQIndex& i1)
    {
    boost::array<IQIndex,NMAX> inds =
        {{ i1, IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null(), 
           IQIndex::Null(), IQIndex::Null() }};

    trace(inds,1);
    }

Real
trace(IQTensor T)
    {
    boost::array<IQIndex,NMAX> inds;
    for(int k = 1; k <= T.r(); ++k)
        {
        inds[k-1] = T.index(k);
        }
    T.trace(inds,T.r());
    return T.toReal();
    }

int IQTensor::
vecSize() const
	{
    if(this->isNull()) return 0;
	int s = 0;
    Foreach(const ITensor& t, *p)
	    s += t.vecSize();
	return s;
	}

int IQTensor::
maxSize() const
	{
    int ms = 1;
	for(int j = 0; j < is_->r(); ++j)
	    ms *= is_->m(j);
    return ms;
    }

void IQTensor::
assignToVec(VectorRef v) const
	{
	if(vecSize() != v.Length())
	    Error("Mismatched sizes in IQTensor::assignToVec(VectorRef v).");
	int off = 1;
    Foreach(const ITensor& t, blocks())
	    {
	    int d = t.vecSize();
	    t.assignToVec(v.SubVector(off,off+d-1));
	    off += d;
	    }
	}

void IQTensor::
assignFromVec(VectorRef v)
	{
	soloDat();
	if(vecSize() != v.Length())
	    Error("bad size");
	int off = 1;
    Foreach(ITensor& t, *p)
	    {
	    int d = t.vecSize();
	    t.assignFromVec(v.SubVector(off,off+d-1));
	    off += d;
	    }
	}

void IQTensor::
randomize() 
	{ 
    if(isNull())
        Error("Can't randomize default constructed IQTensor.");
    if(p->empty())
        Error("Can't randomize IQTensor having no blocks");

	soloDat(); 

    const QN D = div();

    QCounter C(*is_);

    for(;C.notDone();++C)
        {
        QN nd;
        for(int n = 1; n <= r(); ++n)
            {
            nd += is_->index(n).qn(C.i[n]);
            }
        if(nd != D) continue;

        IndexSet<Index> nset;
        for(int n = 1; n <= r(); ++n)
            {
            nset.addindex(is_->index(n).index(C.i[n]));
            }

        ApproxReal r(nset.uniqueReal());
        if(p->has_itensor(r))
            {
            p->get(r).randomize();
            }
        else
            {
            ITensor t(nset);
            t.randomize();
            p->insert_add(r,t);
            }
        }
	}

void IQTensor::
print(std::string name,Printdat pdat) const 
	{ 
    bool savep = Global::printdat();
    Global::printdat() = (pdat==ShowData); 
	std::cerr << "\n" << name << " =\n" << *this << "\n"; 
    Global::printdat() = savep;
	}

void IQTensor::
printIndices(const std::string& name) const
	{ 
	cout << "\n" << name << " (IQIndices only) = \n";
	cout << "/--------------------------------------\n" << endl;
    if(this->isNull())
        {
        cout << "    [IQTensor is null]" << endl;
        return;
        }
    cout << (*is_);
	cout << "\\-------------------------------------\n" << endl;
	}


void IQTensor::
conj()
    {
    if(!isComplex(*this))
        {
        soloIndex();
        is_->conj();
        return;
        }
    else
        {
        soloDat();

        //IQTensor r,i;
        //splitReIm(r,i);
        IQTensor r(realPart(*this)),
                i(imagPart(*this));

        r.is_->conj();
        i.is_->conj();

        i *= -1.0;
        *this = r * IQTensor::Complex_1() + IQTensor::Complex_i() * i;
        }
    }

void IQTensor::
conj(const IQIndex& I)
    {
    soloIndex();
    is_->conj(I);
    }

void IQTensor::
swap(IQTensor& other)
    {
    is_.swap(other.is_);
    p.swap(other.p);
    }

std::ostream& 
operator<<(std::ostream & s, const IQTensor& T)
    {
	s << "/--------------IQTensor--------------\n";
    if(T.isNull())
        {
        s << "     (IQTensor is null)\n\n";
        return s;
        }
    s << "IQIndices:\n";
    for(int k = 1; k <= T.r(); ++k)
        { s << "  " << T.index(k) << std::endl; }
    s << "ITensors:\n";
    Foreach(const ITensor& t, T.blocks())
        { s << "  " << t << std::endl; }
	s << "\\------------------------------------\n\n";
    return s;
    }

IQTensor& IQTensor::
operator*=(const IQTensor& other)
    {
    //TODO: account for fermion sign here
    if(this == &other)
        {
        IQTensor cp_oth(other);
        return operator*=(cp_oth);
        }

    if(this->isNull()) 
        Error("'This' IQTensor null in product");

    if(other.isNull()) 
        Error("Multiplying by null IQTensor");

    if(hasindex(IQIndex::IndReIm()) && other.hasindex(IQIndex::IndReIm()) && !other.hasindex(IQIndex::IndReImP())
	    && !other.hasindex(IQIndex::IndReImPP()) && !hasindex(IQIndex::IndReImP()) && !hasindex(IQIndex::IndReImPP()))
        {
        this->prime(ReIm);
        operator*=(ComplexProd() * primed(other,ReIm,2));
        return *this;
        }

    solo();

    set<ApproxReal> common_inds;
    
    //Load iqindex_ with those IQIndex's *not* common to *this and other
    array<IQIndex,NMAX> riqind_holder;
    int rholder = 0;

    for(int i = 1; i <= is_->r(); ++i)
        {
        const IQIndex& I = is_->index(i);
        const_iqind_it f = find(other.is_->begin(),other.is_->end(),I);
        if(f != other.is_->end()) //I is an element of other.iqindex_
            {
            //Check that arrow directions are compatible
            if(Global::checkArrows())
                if(f->dir() == I.dir() && f->type() != ReIm && I.type() != ReIm)
                    {
                    this->printIndices("*this");
                    other.printIndices("other");
                    cout << "IQIndex from *this = " << I << endl;
                    cout << "IQIndex from other = " << *f << endl;
                    cout << "Incompatible arrow directions in IQTensor::operator*=" << endl;
                    throw ArrowError("Incompatible arrow directions in IQTensor::operator*=.");
                    }
            for(size_t n = 0; n < I.iq().size(); ++n) 
                { common_inds.insert(ApproxReal(I.iq()[n].index.uniqueReal())); }

            common_inds.insert(ApproxReal(I.uniqueReal()));
            }
        else 
            { 
            riqind_holder[rholder] = I;
            ++rholder;
            }
        }

    for(int i = 1; i <= other.is_->r(); ++i)
        {
        const IQIndex& I = other.is_->index(i);
        if(!common_inds.count(ApproxReal(I.uniqueReal())))
            { 
            riqind_holder[rholder] = I;
            ++rholder;
            }
        }

    is_ = make_shared<IndexSet<IQIndex> >(riqind_holder,rholder,0);

    set<ApproxReal> keys;

    list<ITensor> old_itensor; 
    p->swap(old_itensor);

    //com_this maps the uniqueReal of a set of Index's to be contracted over together
    //to those ITensors in *this.itensor having all Index's in that set
    multimap<ApproxReal,IQTDat::const_iterator> com_this;
    for(IQTDat::const_iterator tt = old_itensor.begin(); tt != old_itensor.end(); ++tt)
        {
        Real r = 0.0;
        for(int a = 1; a <= tt->r(); ++a)
            {
            if(common_inds.count(ApproxReal(tt->index(a).uniqueReal())))
                { r += tt->index(a).uniqueReal(); }
            }
        com_this.insert(make_pair(ApproxReal(r),tt));
        keys.insert(ApproxReal(r));
        }

    //com_other is the same as com_this but for other
    multimap<ApproxReal,IQTDat::const_iterator> com_other;
    for(IQTDat::const_iterator ot = other.p->begin(); ot != other.p->end(); ++ot)
        {
        Real r = 0.0;
        for(int b = 1; b <= ot->r(); ++b)
            {
            if(common_inds.count(ApproxReal(ot->index(b).uniqueReal())))
                { r += ot->index(b).uniqueReal(); }
            }
        com_other.insert(make_pair(ApproxReal(r),ot));
        keys.insert(ApproxReal(r));
        }

    typedef multimap<ApproxReal,const_iten_it>::iterator mit;
    pair<mit,mit> lrange,rrange;
    ITensor tt;
    for(set<ApproxReal>::iterator k = keys.begin(); k != keys.end(); ++k)
        {
        //Equal range returns the begin and end iterators for the sequence
        //corresponding to multimap[key] as a pair
        lrange = com_this.equal_range(*k);
        rrange = com_other.equal_range(*k);

        //Iterate over all ITensors in *this and other sharing
        //the set of contracted Index's corresponding to k
        for(mit ll = lrange.first; ll != lrange.second; ++ll)
        for(mit rr = rrange.first; rr != rrange.second; ++rr)
            {
            //Multiply the ITensors and add into res
            tt = *(ll->second); tt *= *(rr->second);
            if(tt.scale().sign() != 0)
                p->insert_add(tt);
            }
        }

    return *this;

    } //IQTensor& IQTensor::operator*=(const IQTensor& other)

IQTensor& IQTensor::
operator/=(const IQTensor& other)
    {
    //TODO: account for fermion sign here
    if(this == &other)
        {
        IQTensor cp_oth(other);
        return operator/=(cp_oth);
        }

    if(this->isNull()) 
        Error("'This' IQTensor null in product");

    if(other.isNull()) 
        Error("Multiplying by null IQTensor");

    /*
    if(hasindex(IQIndex::IndReIm()) && other.hasindex(IQIndex::IndReIm()) && !other.hasindex(IQIndex::IndReImP())
	    && !other.hasindex(IQIndex::IndReImPP()) && !hasindex(IQIndex::IndReImP()) && !hasindex(IQIndex::IndReImPP()))
        {
        Error("IQTensor::operator/= not yet implemented for complex numbers");
        }
        */


    set<ApproxReal> common_inds;
    
    array<IQIndex,NMAX> riqind_holder;
    int rholder = 0;

    for(int i = 1; i <= is_->r(); ++i)
        {
        const IQIndex& I = is_->index(i);
        const_iqind_it f = find(other.is_->begin(),other.is_->end(),I);
        if(f != other.is_->end()) //I is an element of other.iqindex_
            {
            //Check that arrow directions are compatible
            if(Global::checkArrows())
                if(f->dir() != I.dir() && f->type() != ReIm && I.type() != ReIm)
                    {
                    this->printIndices("*this");
                    other.printIndices("other");
                    cout << "IQIndex from *this = " << I << endl;
                    cout << "IQIndex from other = " << *f << endl;
                    cout << "Incompatible arrow directions in IQTensor::operator*=" << endl;
                    throw ArrowError("Incompatible arrow directions in IQTensor::operator/=.");
                    }
            for(size_t n = 0; n < I.iq().size(); ++n) 
                { common_inds.insert(ApproxReal(I.iq()[n].index.uniqueReal())); }

            common_inds.insert(ApproxReal(I.uniqueReal()));
            }
        riqind_holder[rholder] = I;
        ++rholder;
        }

    bool inds_from_other = false;
    for(int i = 1; i <= other.is_->r(); ++i)
        {
        const IQIndex& I = other.is_->index(i);
        if(!common_inds.count(ApproxReal(I.uniqueReal())))
            { 
            riqind_holder[rholder] = I;
            ++rholder;
            inds_from_other = true;
            }
        }

    //Only update IQIndices if they are different
    //from current set
    if(inds_from_other)
        {
        is_ = make_shared<IndexSet<IQIndex> >(riqind_holder,rholder,0);
        }


    soloDat();

    list<ITensor> old_itensor; 
    p->swap(old_itensor);

    set<ApproxReal> keys;

    //com_this maps the uniqueReal of a set of Index's to be summed over together
    //to those ITensors in *this.itensor having all Index's in that set
    multimap<ApproxReal,IQTDat::const_iterator> com_this;
    for(IQTDat::const_iterator tt = old_itensor.begin(); tt != old_itensor.end(); ++tt)
        {
        Real r = 0.0;
        for(int a = 1; a <= tt->r(); ++a)
            {
            Real ur = tt->index(a).uniqueReal();
            if(common_inds.count(ApproxReal(ur)))
                r += ur;
            }
        com_this.insert(make_pair(ApproxReal(r),tt));
        keys.insert(ApproxReal(r));
        }

    //com_other is the same as com_this but for other
    //Cheaper just to store IQTensors? (since they are just two pointers?)
    multimap<ApproxReal,IQTDat::const_iterator> com_other;
    for(IQTDat::const_iterator ot = other.p->begin(); ot != other.p->end(); ++ot)
        {
        Real r = 0.0;
        for(int b = 1; b <= ot->r(); ++b)
            {
            Real ur = ot->index(b).uniqueReal();
            if(common_inds.count(ApproxReal(ur)))
                r += ur;
            }
        com_other.insert(make_pair(ApproxReal(r),ot));
        keys.insert(ApproxReal(r));
        }

    typedef multimap<ApproxReal,const_iten_it>::iterator mit;
    pair<mit,mit> lrange,rrange;
    ITensor tt;
    for(set<ApproxReal>::iterator k = keys.begin(); k != keys.end(); ++k)
        {
        //Equal range returns the begin and end iterators for the sequence
        //corresponding to multimap[key] as a pair
        lrange = com_this.equal_range(*k);
        rrange = com_other.equal_range(*k);

        //Iterate over all ITensors in *this and other sharing
        //the set of contracted Index's corresponding to k
        for(mit ll = lrange.first; ll != lrange.second; ++ll)
        for(mit rr = rrange.first; rr != rrange.second; ++rr)
            {
            //Multiply the ITensors and add into res
            tt = *(ll->second); tt /= *(rr->second);
            if(tt.scale().sign() != 0)
                p->insert_add(tt);
            }
        }

    return *this;

    } //IQTensor& IQTensor::operator/=(const IQTensor& other)

//Extracts the real and imaginary parts of the 
//component of a rank 0 tensor (scalar)
void IQTensor::
toComplex(Real& re, Real& im) const
    {
    if(isComplex(*this))
        {
        re = realPart(*this).toReal();
        im = imagPart(*this).toReal();
        }
    else
        {
        re = toReal();
        im = 0;
        }
    }

Real IQTensor::
toReal() const
    {
    if(is_->r() != 0)
        Error("IQTensor not a real scalar");
#ifdef DEBUG
    if(iten_size() > 1)
        Error("Too many blocks");
#endif
    if(iten_empty())
        return 0;
    else
        return p->begin()->toReal();
    }

IQTensor& IQTensor::
operator+=(const IQTensor& other)
    {
    //TODO: account for fermion sign here
    if(this == &other) 
        {
        operator*=(2);
        return *this;
        }

    IQTensor& This = *this;
    if(fabs(This.uniqueReal()-other.uniqueReal()) > 1.0e-11) 
        {
        PrintIndices(This);
        PrintIndices(other);
        Print(This.uniqueReal());
        Print(other.uniqueReal());
        Error("Mismatched indices in IQTensor::operator+=");
        }

    if(is_->r() == 0)	// Automatic initializing a summed IQTensor in a loop
        { 
        return (*this = other); 
        }

    bool complex_this = hasindex(IQIndex::IndReIm()); 
    bool complex_other = other.hasindex(IQIndex::IndReIm()); 
    if(!complex_this && complex_other)
        return (This = (This * IQTensor::Complex_1()) + other);
    if(complex_this && !complex_other)
        return (This += other * IQTensor::Complex_1());

    soloDat(); 

    Foreach(const ITensor& t, *(other.p))
        { 
        p->insert_add(t);
        }

    return *this;
    }

//
//Automatically convert this IQTensor
//to an ITensor
//
ITensor IQTensor::
toITensor() const
    {
    //Resulting ITensor's indices are 
    //the Index versions of this's IQIndices
    vector<Index> indices;
    for(int j = 1; j <= is_->r(); ++j)
        {
        indices.push_back(Index(is_->index(j)));
        }
    ITensor res(indices);

    //Loop over ITensors (blocks) within this IQTensor
    Foreach(const ITensor& t, *p)
        {
        ITensor exp(t);
        //Loop over Index's of the k'th ITensor
        for(int j = 1; j <= t.r(); ++j)
            {
            //Want to transform 'small' into the 
            //Index version of the IQIndex that contains
            //it, with the appropriate offset
            const Index& small = t.index(j);
            //Find the IQIndex that contains 'small'
            const IQIndex* big = 0;
            int offset = -1;
            for(int q = 1; q <= is_->r(); ++q)
                if(is_->index(q).hasindex(small))
                    {
                    big = &(is_->index(q));
                    offset = big->offset(small);
                    break;
                    }
            exp.expandIndex(small,*big,offset);
            }
        //Once all Indices expanded, add to res
        res += exp;
        }
    return res;
    } //IQTensor::operator ITensor() const

void IQTensor::
soloIndex()
	{
	if(!is_)
        Error("IQTensor is null");

	if(!is_.unique())
        is_ = make_shared<IndexSet<IQIndex> >(*is_);
    }

void IQTensor::
soloDat()
    {
	if(!p)
        Error("IQTensor is null");

	if(!p.unique())
        p = make_shared<IQTDat>(*p);
	}

void IQTensor::
solo()
    {
    soloIndex();
    soloDat();
    }


Real 
Dot(IQTensor x, const IQTensor& y)
    {
    IQIndex I = commonIndex(x,y);
    if(I.dir() == dir(y.indices(),I))
        {
        x.conj();
        }
    x *= y;
    return x.toReal();
    }

void 
BraKet(IQTensor x, const IQTensor& y, Real& re, Real& im)
    {
    x.conj();
    x *= y;
    x.toComplex(re,im);
    }

void 
checkQNs(const IQTensor& T)
    {
    QN qtot = T.div();
    Foreach(const ITensor& t, T.blocks())
        {
        QN q;
        for(int j = 1; j <= t.r(); ++j) 
            q += T.qn(t.index(j))*T.dir(t.index(j));

        if(q != qtot) 
            {
            std::cout << "\nqtot = " << qtot << "\n\n";
            std::cout << "Offending ITensor = " << t << "\n\n";
            T.printIndices("T");
            Error("checkQNs: inconsistent QN");
            }
        }
    }

void 
checkDiv(const IQTensor& T, QN expected)
	{
	if(T.p->empty())
	    {   
	    T.printIndices("this");
	    Error("IQTensor has no blocks");
	    }

    Foreach(const ITensor& t, *(T.p))
	    {
	    QN div_;
	    for(int j = 1; j <= t.r(); ++j)
            div_ += T.qn(t.index(j))*T.dir(t.index(j));
	    if(div_ != expected)
            {
            Print(expected);
            Print(div_);
            T.printIndices("This IQTensor:");
            cout << "Incorrect block:\n";
            Print(t);
            Error("Block didn't match expected div");
            }
	    }
	}
