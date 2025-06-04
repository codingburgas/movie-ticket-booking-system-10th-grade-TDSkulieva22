#pragma once
#include "pch.h"

struct OfferDetails {
    int offerId;
    std::wstring title;
    std::wstring description;
    bool isActive;
};