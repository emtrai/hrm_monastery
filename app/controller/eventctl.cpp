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
 * Filename: eventctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/19/2022
 * Brief:
 */
#include "eventctl.h"

#include "logger.h"
#include "event.h"
#include "dbctl.h"
#include "utils.h"
#include "defs.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"

GET_INSTANCE_CONTROLLER_IMPL(EventCtl)

EventCtl::EventCtl():ModelController(KControllerEvent, KModelHdlEvent)
{
    tracein;
}

EventCtl::~EventCtl()
{
    tracein;
}

const char *EventCtl::getPrebuiltFileName()
{
    return KPrebuiltEventCSVFileName;
}

DbModelBuilder EventCtl::getMainBuilder()
{
    return &Event::build;
}
