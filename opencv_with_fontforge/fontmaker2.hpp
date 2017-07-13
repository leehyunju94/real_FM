//
//  fontmaker2.hpp
//  test-opencv
//
//  Created by 유원상 on 2017. 7. 12..
//  Copyright © 2017년 유원상. All rights reserved.
//

#ifndef fontmaker2_hpp
#define fontmaker2_hpp

#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>

using square_t   = std::pair <cv::Range, cv::Range>;
using contour_t  = std::vector <cv::Point>;
using contours_t = std::vector <std::vector <cv::Point>>;
using str_t      = std::string;

struct square_comp
{
    bool operator()(const square_t& l, const square_t& r) const
    {
        if (l.first.end < r.first.start)
        {
            return true;
        }
        else if ((l.first.start < r.first.end) && (l.second.start < r.second.start))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

using location_info_t = std::map <square_t, contour_t, square_comp>;

contours_t extract_contours (cv::Mat origin)
{
    cv::Mat gray {};
    cv::cvtColor(origin, gray, cv::COLOR_RGB2GRAY);
    if (gray.empty())
    {
        fmt::print("[fail] cvt gray scale\n");
        exit(1);
    }
    
    cv::Mat blur {};
    cv::GaussianBlur(gray, blur, cv::Size{5,5}, 0);
    if (blur.empty())
    {
        fmt::print("[fail] gaussian blur\n");
        exit(1);
    }
    
    cv::Mat threshold {};
    cv::adaptiveThreshold(blur, threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
    if (threshold.empty())
    {
        fmt::print("[fail] threshold\n");
        exit(1);
    }
    
    contours_t result {};
    cv::findContours(threshold, result, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    if (result.empty())
    {
        fmt::print("[fail] find contours\n");
        exit(1);
    }
    
    return result;
}

auto filter_contours (contours_t contours, size_t bound)
{
    struct square_comp
    {
        bool operator()(square_t l, square_t r)
        {
            if (l.first.end < r.first.start)
                return true;
            else if ((l.first.start < r.first.end) && (l.second.start < r.second.start))
                return true;
            else
                return false;
        }
    };
    location_info_t result;
    
    for (auto contour : contours)
    {
        cv::Range r { INT_MAX, 0 };
        cv::Range c { INT_MAX, 0 };
        
        for (auto point : contour)
        {
            r.start = std::min(r.start, point.y);
            r.end   = std::max(r.end,   point.y);
            
            c.start = std::min(c.start, point.x);
            c.end   = std::max(c.end,   point.x);
        }
        
        int r_len = r.size();
        int c_len = c.size();
        
        if ((r_len > 0) && (c_len > 0))
        {
            if ((r_len + c_len) > bound)
            {
                if (result.emplace(std::make_pair(r, c), contour).second == false)
                {
                    fmt::print("[fail] data insert into result using emplace func\n");
                    exit(1);
                }
            }
            else
            {
                fmt::print("[except] not enough contour size\n");
            }
        }
        else
        {
            fmt::print("[except] invalid contour size\n");
        }
        
    }
    
    return result;
}



std::map<char, cv::Mat> make_charset (cv::Mat origin, std::string predict, location_info_t charset_info)
{
    std::map<char, cv::Mat> charset;
    
    auto c   = predict.begin();
    auto end = predict.end();
    
    for (auto char_info : charset_info)
    {
        while(true)
        {
            if (*c == ' ')
                ++c;
            else
                break;
        }
        
        if (c == end)
        {
            break;
        }
        
        contour_t contour_sorted_y = char_info.second;
        contour_t contour_sorted_x = contour_sorted_y;
        
        std::sort(contour_sorted_y.begin(), contour_sorted_y.end(),
                  [](const cv::Point& l, const cv::Point& r)
                  {
                      if (l.y > r.y)
                          return false;
                      else if (l.y < r.y)
                          return true;
                      else
                          return l.x < r.x;
                  });
        std::sort(contour_sorted_x.begin(), contour_sorted_x.end(),
                  [](const cv::Point& l, const cv::Point& r)
                  {
                      if (l.x > r.x)
                          return false;
                      else if (l.x < r.x)
                          return true;
                      else
                          return l.y < r.y;
                  });
        
        cv::Range row = char_info.first.first;
        cv::Range col = char_info.first.second;
        
        --row.start;
        ++row.end;
        --col.start;
        ++col.end;
        
        auto ref = origin.clone();
        auto char_img = ref(row, col);
        
        /* 테두리 제거하기 알고리즘 */
        // -- 시작 --
        using contour_iter_t = decltype(contour_sorted_y.begin());

        auto cvt_AB2REL = [row, col](int y, int x) -> cv::Point {
            return { y - row.start, x - col.start };
        };

        auto calc_x_range = [](contour_iter_t begin, contour_iter_t end,
                               int y) -> std::pair<cv::Range, contour_iter_t> {
            cv::Range range {INT_MAX, 0};
            
            if ((*begin).y > y)
                return {range, begin};
            
            for (;begin < end; ++begin)
            {
                if ((*begin).y == y)
                {
                    range.start = std::min(range.start, (*begin).x);
                }
                else if ((*begin).y > y)
                {
                    --begin;
                    range.end = (*begin).x;
                    ++begin;
                    break;
                }
            }
            return {range, begin};
        };
        
        auto iter = contour_sorted_y.begin();
        auto end  = contour_sorted_y.end();
        
        for (int y = 0; y < row.size(); ++y)
        {
            fmt::print("[DEBUG] {} > {}\n", (*iter).y, y + row.start);
            if (iter >= end)
            {
                fmt::print("[fail] overflow \n");
                exit(1);
            }
            else if ((*iter).y > (y + row.start))
            {
                for (int x = 0; x < col.size(); ++x)
                {
                    char_img.at<cv::Vec3b>(y, x) = {255, 255, 255};
                }
            }
            else if ((*iter).y == (y + row.start))
            {
                auto x_info = calc_x_range(iter, end, (*iter).y);
                auto x_range = x_info.first;
                fmt::print("[DEBUG] y:{} range = {} to {}\n", y + row.start, x_range.start, x_range.end);
                iter = x_info.second;
                
                
                for (int x = 0; x < x_range.start - col.start; ++x)
                {
                    char_img.at<cv::Vec3b>(y, x) = {255, 255, 255};
                }
                for (int x = col.size(); x > x_range.end - col.start; --x)
                {
                    char_img.at<cv::Vec3b>(y, x) = {255, 255, 255};
                }
            }
        }
        
        auto calc_y_range = [](contour_iter_t begin, contour_iter_t end,
                               int x) -> std::pair<cv::Range, contour_iter_t> {
            cv::Range range {INT_MAX, 0};
            
            if ((*begin).x > x)
                return {range, begin};
            
            for (;begin < end; ++begin)
            {
                if ((*begin).x == x)
                {
                    range.start = (*begin).y;
                }
                else if ((*begin).x > x)
                {
                    --begin;
                    range.end = (*begin).y;
                    ++begin;
                }
            }
            return {range, begin};
        };

        iter = contour_sorted_x.begin();
        end  = contour_sorted_x.end();

        for (int x = 0; x < col.size(); ++x)
        {
            fmt::print("iter: {},{} end: {},{}\n", (*iter).y, (*iter).x, (*end).y, (*end).x);
            fmt::print("[DEBUG] {} > {}\n", (*iter).x, x + col.start);
            if (iter >= end)
            {
                fmt::print("[fail] overflow \n");
//                exit(1);
            }
            else if ((*iter).x > (x + col.start))
            {
                for (int y = 0; y < row.size(); ++y)
                {
                    char_img.at<cv::Vec3b>(y, x) = {255, 255, 255};
                }
            }
            else if ((*iter).x == (x + col.start))
            {
                auto y_info = calc_y_range(iter, end, (*iter).x);
                auto y_range = y_info.first;
                fmt::print("[DEBUG] y:{} range = {} to {}\n", x + col.start, y_range.start, y_range.end);
                iter = y_info.second;
                
                
                for (int y = 0; y < y_range.start - row.start; ++y)
                {
                    char_img.at<cv::Vec3b>(y, x) = {255, 255, 255};
                }
                for (int y = row.size(); y > y_range.end - row.start; --y)
                {
                    char_img.at<cv::Vec3b>(y, x) = {255, 255, 255};
                }
            }
        }
        // --  끝 --
// 색 채우기 방법: char_img.at<cv::Vec3b>(y, x) = { 0, 0, 0 };
        
        std::string char_case = (*c < 'a')? "upper":"lower";
        cv::imwrite(fmt::format("/Users/yws/Desktop/output/{}_{}.png", char_case, *c), char_img);
        
        fmt::print("[DEBUG] {}_{} \n", char_case, *c);
        for (auto point : contour_sorted_y)
        {
            fmt::print("({},{}) ", point.y, point.x);
        }
        fmt::print("\n\n");
        ++c;
    }
    cv::imwrite(fmt::format("/Users/yws/Desktop/output/origin.png"), origin);
    return charset;
}

#endif /* fontmaker2_hpp */
