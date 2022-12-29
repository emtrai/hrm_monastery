/*
 * Copyright (C) 2022 Ngo Huy Anh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * Filename: dlgcommonmodel.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/29/2022
 * Brief:
 */
#include "dlgcommonmodel.h"
#include "logger.h"
#include "dbmodel.h"

DlgCommonModel::DlgCommonModel():mModel(nullptr), mIsNew(false), mIsSelfSave(false)
{

}

DlgCommonModel::~DlgCommonModel()
{
}

bool DlgCommonModel::isNew() const
{
    return mIsNew;
}

void DlgCommonModel::setIsNew(bool newIsNew)
{
    mIsNew = newIsNew;
}

bool DlgCommonModel::isSelfSave() const
{
    return mIsSelfSave;
}

void DlgCommonModel::setIsSelfSave(bool newIsSelfSave)
{
    mIsSelfSave = newIsSelfSave;
}

DbModel *DlgCommonModel::model()
{
    if (!mModel) {
        mModel = newModel();
    }
    // TODO: copy or return value??? how will take over this object??? should use smart pointer????
    return mModel;
}

ErrCode DlgCommonModel::fromModel(const DbModel *inModel)
{
    traced;
    ErrCode ret = ErrNone;
    if (inModel == nullptr) {
        loge("Invalid model info to clone");
        return ErrInvalidData;
    }
    DbModel* item = model();
    item->clone(inModel);
    item->validate(); // TODO: should call validate here???
    if (item == nullptr){
        ret = ErrInvalidArg; // TODO: should raise assert instead???
    }
    tracedr(ret);
}
