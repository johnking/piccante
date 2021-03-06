/*

PICCANTE
The hottest HDR imaging library!
http://vcg.isti.cnr.it/piccante

Copyright (C) 2014
Visual Computing Laboratory - ISTI CNR
http://vcg.isti.cnr.it
First author: Francesco Banterle

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef PIC_METRICS_SNR_HPP
#define PIC_METRICS_SNR_HPP

#include "image.hpp"

namespace pic {

/**
 * @brief SNR computes the signal to noise ratio between two images.
 * @param ori is the original image.
 * @param cmp is the distorted image.
 * @param box is a bounding box where to limit the computation of SNR.
 * @return It returns the SNR value between ori and cmp.
 */
float SNR(Image *ori, Image *cmp, BBox *box = NULL)
{
    if(ori == NULL || cmp == NULL) {
        return -2.0f;
    }

    if(!ori->isSimilarType(cmp)) {
        return -1.0f;
    }

    if(box == NULL) {
        box = new BBox(ori->width, ori->height);
    }

    double acc = 0.0;

    for(int j = box->y0; j < box->y1; j++) {
        for(int i = box->x0; i < box->x1; i++) {
            int c = i * ori->xstride + j * ori->ystride;

            for(int k = 0; k < ori->channels; k++) {
                double valO = static_cast<double>(ori->data[c + k]);
                double valC = static_cast<double>(cmp->data[c + k]);

                if(valO > 1e-3) {// small values are skipped to avoid numerical problems
                    double tmp = fabs(valO - valC);
                    acc += tmp / valO;
                }
            }
        }
    }

    acc /= static_cast<double>((box->y1 - box->y0) * (box->x1 - box->x0) *
                               ori->channels);

    float snr = static_cast<float>(-20.0 * log10(acc));

    // some numerical error in the filtered image occur..
    snr = (snr > 200.0 || snr < 1.0) ? -1.0f : snr;

    return snr;
}

} // end namespace pic

#endif /* PIC_METRICS_SNR_HPP */
