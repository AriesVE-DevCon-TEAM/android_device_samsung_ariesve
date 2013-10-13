/*
 * Copyright (C) 2008 The Android Open Source Project
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
 */

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/properties.h>
#include <cutils/log.h>


#include "OrientationSensor.h"


/*****************************************************************************/
OrientationSensor::OrientationSensor()
    : SensorBase(NULL, "orientation"),
      mEnabled(0),
      mInputReader(4),
      mHasPendingEvent(false)
{
    ALOGD("OrientationSensor::OrientationSensor()");
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_O;
    mPendingEvent.type =  SENSOR_TYPE_ORIENTATION;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));
    
    ALOGD("OrientationSensor::OrientationSensor() open data_fd");

    if (data_fd) {
        strcpy(input_sysfs_path, "/sys/class/input/");
        strcat(input_sysfs_path, input_name);
        strcat(input_sysfs_path, "/device/");
        input_sysfs_path_len = strlen(input_sysfs_path);

       // enable(0, 1);
    }
}

OrientationSensor::~OrientationSensor() {

    ALOGD("OrientationSensor::~OrientationSensor()");
    if (mEnabled) {
        enable(0, 0);
    }
}


int OrientationSensor::enable(int32_t, int en) {

    ALOGD("OrientationSensor::~enable(0, %d)", en);
    int flags = en ? 1 : 0;
    if (flags != mEnabled) {
        int fd;
        strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
        ALOGD("OrientationSensor::~enable(0, %d) open %s",en,  input_sysfs_path);
        fd = open(input_sysfs_path, O_RDWR);
        if (fd >= 0) {
             ALOGD("OrientationSensor::~enable(0, %d) opened %s",en,  input_sysfs_path);
            char buf[2];
            int err;
            buf[1] = 0;
            if (flags) {
                buf[0] = '1';
            } else {
                buf[0] = '0';
            }
            err = write(fd, buf, sizeof(buf));
            close(fd);
            mEnabled = flags;
            //setInitialState();

            /* Since the migration to 3.0 kernel, orientationd doesn't poll
             * the enabled state properly, so start it when it's enabled and
             * stop it when we're done using it.
             */
            property_set(mEnabled ? "ctl.start" : "ctl.stop", "orientationd");
            return 0;
        }
        return -1;
    }
    return 0;
}


bool OrientationSensor::hasPendingEvents() const {
    /* FIXME probably here should be returning mEnabled but instead
	mHasPendingEvents. It does not work, so we cheat.*/
    //ALOGD("OrientationSensor::~hasPendingEvents %d", mHasPendingEvent ? 1 : 0 );
    return mHasPendingEvent;
}


int OrientationSensor::setDelay(int32_t handle, int64_t ns)
{
    ALOGD("OrientationSensor::~setDelay(%d, %lld)", handle, ns);

    int fd;

    if (ns < 10000000) {
        ns = 10000000; // Minimum on stock
    }

    strcpy(&input_sysfs_path[input_sysfs_path_len], "delay");
    fd = open(input_sysfs_path, O_RDWR);
    if (fd >= 0) {
        char buf[80];
        sprintf(buf, "%lld", ns / 10000000 * 10); // Some flooring to match stock value
        write(fd, buf, strlen(buf)+1);
        close(fd);
        return 0;
    }
    return -1;
}


int OrientationSensor::readEvents(sensors_event_t* data, int count)
{
    //ALOGD("OrientationSensor::~readEvents() %d", count);
    if (count < 1)
        return -EINVAL;
        
    if (mHasPendingEvent) {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled ? 1 : 0;
    }
        
    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_ABS) {
            float value = event->value;
            if (event->code == EVENT_TYPE_YAW) {
                mPendingEvent.orientation.azimuth = value * CONVERT_O_A;
            } else if (event->code == EVENT_TYPE_PITCH) {
                mPendingEvent.orientation.pitch = value * CONVERT_O_P;
            } else if (event->code == EVENT_TYPE_ROLL) {
                mPendingEvent.orientation.roll = value * CONVERT_O_R;
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("OrientationSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

	//ALOGD("OrientationSensor::~readEvents() numEventReceived = %d", numEventReceived);
    return numEventReceived++;
}
