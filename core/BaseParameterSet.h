//
//  BaseParameterSet.hpp
//  UiTest
//
//  Created by Олег on 22.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef BaseParameterSet_hpp
#define BaseParameterSet_hpp

#include <vector>
#include "IParameterSet.h"
#include "BaseParameters.h"
#include "ParameterHelpers.h"

class BaseParameterSet : public IParameterSet
{
public:

    BaseParameterSet( const BaseParameterSet& other ) = delete;
    BaseParameterSet& operator=( const BaseParameterSet& ) = delete;
    BaseParameterSet( BaseParameterSet&& other );
    BaseParameterSet& operator=( BaseParameterSet&& other );
    
    BaseParameterSet(bool manageData = false);
    
    virtual ~BaseParameterSet();
    
    // @return parameter from set.
    virtual const Parameter& value(index_t index) const override;
    
    // @return parameters number.
    virtual index_t parametersNumber() const override;
    
    
    // Needed methods from vector.
    void push_back( BaseParameters type, const Parameter& value );
    void push_back( BaseParameters type, Parameter&& value );
    
    template <typename T> void push_back( NumberParameter<T>&& value )
    {
        push_back(NumberParameterInfo<T>::paramType(), value);
    }
    
    const Parameter& front() const;
    void clear();
    size_t size() const;
    const Parameter& at( size_t pos ) const;
    void replace(size_t pos, const Parameter& newValue);

protected:

    using ParamAndType = std::pair<BaseParameters, Parameter>;
    std::vector<ParamAndType> data;
    
    // Is this class manage data or not.
    bool manageData;
    
    void preParameterRemove(ParamAndType& parameter);
};

typedef std::shared_ptr<BaseParameterSet> BaseParameterSetPtr;

#endif /* BaseParameterSet_hpp */
