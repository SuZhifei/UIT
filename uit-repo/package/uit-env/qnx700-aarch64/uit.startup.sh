#!/bin/sh

####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# vsomeipd
. ./uit.env.sh

vsomeipd &
uit.svc.data &
uit.uid.cluster-kanzi &
