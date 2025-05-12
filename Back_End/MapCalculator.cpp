#include "MapCalculator.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief 构造函数
 */
MapCalculator::MapCalculator() : field(0, 0) {}

/**
 * @brief 加载场地和音箱数据
 */
bool MapCalculator::loadData(const std::string& path) {
    std::ifstream in_file(path);
    if (!in_file) {
        std::cout << "【错误】无法打开数据文件: " << path << std::endl;
        return false;
    }

    int width = 0, length = 0;
    if (!(in_file >> width >> length) || width <= 0 || length <= 0) {
        std::cout << "【错误】数据文件格式异常或场地尺寸无效。" << std::endl;
        return false;
    }
    field.setWidth(width);
    field.setLength(length);

    int speaker_count = 0;
    if (!(in_file >> speaker_count) || speaker_count < 0) {
        std::cout << "【错误】音箱数量读取失败。" << std::endl;
        return false;
    }

    speakers.clear();
    for (int i = 0; i < speaker_count; ++i) {
        int x = 0, y = 0, sensitivity = 0, impedance = 0, rms_power = 0, coverage_angle = 0, main_axis = 0;
        if (!(in_file >> x >> y >> sensitivity >> impedance >> rms_power >> coverage_angle >> main_axis)) {
            std::cout << "【错误】第" << (i+1) << "个音箱参数读取失败。" << std::endl;
            return false;
        }
        speakers.emplace_back(x, y, sensitivity, impedance, rms_power, coverage_angle, main_axis);
    }
    return true;
}

/**
 * @brief 判断(x, y)处是否为音箱位置
 */
bool MapCalculator::isSpeakerAt(int x, int y) const {
    for (const auto& sp : speakers) {
        if (sp.getX() == x && sp.getY() == y)
            return true;
    }
    return false;
}

/**
 * @brief 计算指定位置的总声压级（不区分频率）
 */
double MapCalculator::getDecibelAt(int x, int y) const {
    constexpr double MIN_DB_CONTRIBUTION = 5.0;
    constexpr double MIN_DISTANCE = 1.0;
    double total_power = 0.0;

    for (const auto& sp : speakers) {
        double dx = static_cast<double>(x - sp.getX());
        double dy = static_cast<double>(y - sp.getY());
        double r = std::max(MIN_DISTANCE, std::sqrt(dx * dx + dy * dy));
        double rms_power = static_cast<double>(sp.getRmsPower());
        if (rms_power <= 0) continue;

        int coverage = sp.getCoverageAngle();
        double di = 0.0;
        if (coverage > 0) {
            double point_angle = std::atan2(dy, dx) * 180.0 / M_PI;
            if (point_angle < 0) point_angle += 360.0;
            double axis_angle = static_cast<double>(sp.getMainAxisOrientation());
            double delta = point_angle + axis_angle;
            while (delta > 180.0) delta -= 360.0;
            while (delta < -180.0) delta += 360.0;
            double abs_delta = std::fabs(delta);

            double theta6dB = coverage / 2.0;
            double n = std::log(0.5) / std::log(std::cos(theta6dB * M_PI / 180.0));
            double theta_rad = abs_delta * M_PI / 180.0;
            double main_lobe = std::pow(std::max(std::cos(theta_rad), 0.0), n);
            di = (main_lobe > 0) ? 20.0 * std::log10(main_lobe) : -40.0;
        }
        // 全指向
        double dB_1m = sp.getSensitivity_dBWm() + 10.0 * std::log10(rms_power) + di;
        double dB_at_point = dB_1m - 20.0 * std::log10(r);

        if (dB_at_point < MIN_DB_CONTRIBUTION) continue;
        total_power += std::pow(10.0, dB_at_point / 10.0);
    }
    return (total_power < 1e-12) ? 0.0 : 10.0 * std::log10(total_power);
}

/**
 * @brief 计算指定位置的总声压级（含频率影响）
 */
double MapCalculator::getDecibelAt(int x, int y, double freqHz) const {
    constexpr double MIN_DB_CONTRIBUTION = 1.0;
    constexpr double MIN_DISTANCE = 1.0;
    double total_power = 0.0;

    for (const auto& sp : speakers) {
        double dx = static_cast<double>(x - sp.getX());
        double dy = static_cast<double>(y - sp.getY());
        double r = std::max(MIN_DISTANCE, std::sqrt(dx * dx + dy * dy));
        double rms_power = static_cast<double>(sp.getRmsPower());
        if (rms_power <= 0) continue;

        int coverage1k = sp.getCoverageAngle();
        bool is_omni = (coverage1k <= 0 || coverage1k >= 360);

        double dB_at_point = 0.0;
        if (is_omni) {
            double freq_loss = 0.0;
            if (freqHz > 0) {
                double alpha = 0.0;
                if (freqHz >= 8000) alpha = 0.093;
                else if (freqHz >= 4000) alpha = 0.028;
                else if (freqHz >= 1000) alpha = 0.003;
                freq_loss = alpha * r;
            }
            double dB_1m = sp.getSensitivity_dBWm() + 10.0 * std::log10(rms_power);
            dB_at_point = dB_1m - 20.0 * std::log10(r) - freq_loss;
        } else {
            double coverage = static_cast<double>(coverage1k);
            if (freqHz > 500) {
                coverage = coverage1k * std::sqrt(1000.0 / freqHz);
                coverage = std::clamp(coverage, 10.0, 180.0);
            }
            double point_angle = std::atan2(dy, dx) * 180.0 / M_PI;
            if (point_angle < 0) point_angle += 360.0;
            double axis_angle = static_cast<double>(sp.getMainAxisOrientation());
            double delta = point_angle + axis_angle;
            while (delta > 180.0) delta -= 360.0;
            while (delta < -180.0) delta += 360.0;
            double abs_delta = std::fabs(delta);

            double di = 0.0;
            if (freqHz <= 500) {
                di = 0.0;
            } else {
                double theta6dB = coverage / 2.0;
                double n = std::log(0.5) / std::log(std::cos(theta6dB * M_PI / 180.0));
                double theta_rad = abs_delta * M_PI / 180.0;
                double main_lobe = std::pow(std::max(std::cos(theta_rad), 0.0), n);
                di = (main_lobe > 0) ? 20.0 * std::log10(main_lobe) : -40.0;
            }

            double dB_1m = sp.getSensitivity_dBWm() + 10.0 * std::log10(rms_power) + di;
            dB_at_point = dB_1m - 20.0 * std::log10(r);
        }
        if (dB_at_point < MIN_DB_CONTRIBUTION) continue;
        total_power += std::pow(10.0, dB_at_point / 10.0);
    }
    return (total_power < 1e-12) ? 0.0 : 10.0 * std::log10(total_power);
}