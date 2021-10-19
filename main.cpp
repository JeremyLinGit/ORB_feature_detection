//
//  main.cpp
//  cvtest2
//
//  Created by Jeremy Lin on 2021/8/1.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
int main(int argc, const char * argv[]) {
    // insert code here...
    cv::Mat img1 = cv::imread("your image path");
    cv::Mat img2 = cv::imread("your image path");
    cv::imshow("image1 point",img1);
    cv::imshow("image2 point",img2);
    cv::waitKey(0);
    
    //initialize
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    cv::Ptr<cv::ORB> orb = cv::ORB::create(500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE,31,20);
    
    // detect feature point, oritented fast
    orb->detect(img1, keypoints1);
    orb->detect(img2, keypoints2);
    
    //compute BRIEF descriptor
    orb->compute(img1, keypoints1, descriptors1);
    orb->compute(img2, keypoints2, descriptors2);
    cv::Mat outimg1,outimg2;
    cv::drawKeypoints(img1, keypoints1, outimg1);
    cv::drawKeypoints(img2, keypoints2, outimg2);
    cv::imshow("ORB feature point1",outimg1);
    cv::imshow("ORB feature point2",outimg2);
    cv::waitKey(0);
    
    std::vector<cv::DMatch> matches;
    cv::BFMatcher matcher ( cv::NORM_HAMMING );
    matcher.match(descriptors1, descriptors2,matches);
    
    //feature points matching
    double min_dist=10000, max_dist=0;
    // find the max and min dis. for all matches, the most and the least similar
    for ( int i = 0; i < descriptors1.rows; i++ )
    {
        double dist = matches[i].distance;
        if ( dist < min_dist ) min_dist = dist;
        if ( dist > max_dist ) max_dist = dist;
    }
    printf ( "-- Max dist : %f \n", max_dist );
    printf ( "-- Min dist : %f \n", min_dist );
    
    // if the dist is greater than 2 times min, could be wrong match. 30 to be lower limit (from experince)
    std::vector< cv::DMatch > good_matches;
    for ( int i = 0; i < descriptors1.rows; i++ )
    {
        if ( matches[i].distance <= cv::max ( 2*min_dist, 30.0 ) )
        {
            good_matches.push_back ( matches[i] );
        }
    }
    
    cv::Mat img_match;
    cv::Mat img_good_match;
    cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, img_match);
    cv::drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_good_match);
    cv::imshow("all match", img_match);
    cv::imshow("good matches", img_good_match);
    cv::waitKey(0);
    return 0;
}
