#include <textures/perlin.h>

Perlin::Perlin()
{
    for (int i = 0; i < point_count; ++i) {
        ranvec[i] = Vec3::random_ranged(-1, 1).unit_vector();
    }

    perlin_generate_perm(perm_x);
    perlin_generate_perm(perm_y);
    perlin_generate_perm(perm_z);
}

double Perlin::noise(const Point3 &p) const
{
    auto u = p.x() - std::floor(p.x());
    auto v = p.y() - std::floor(p.y());
    auto w = p.z() - std::floor(p.z());
    auto i = static_cast<int>(std::floor(p.x()));
    auto j = static_cast<int>(std::floor(p.y()));
    auto k = static_cast<int>(std::floor(p.z()));
    Vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = ranvec[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                ];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}

double Perlin::turb(const Point3 &p, int depth) const
{
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return std::fabs(accum);
}

void Perlin::perlin_generate_perm(std::array<int, Perlin::point_count> &perm)
{
    for (int i = 0; i < point_count; ++i) {
        perm[i] = i;
    }

    permute(perm);
}

void Perlin::permute(std::array<int, Perlin::point_count> &perm)
{
    for (int i = perm.size() - 1; i > 0; i--) {
        int target = random_int(0, i);
        int tmp = perm[i];
        perm[i] = perm[target];
        perm[target] = tmp;
    }
}

double Perlin::perlin_interp(Vec3 c[2][2][2], double u, double v, double w)
{
    auto uu = u*u*(3-2*u);
    auto vv = v*v*(3-2*v);
    auto ww = w*w*(3-2*w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                Vec3 weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu)) *
                        (j*vv + (1-j)*(1-vv)) *
                        (k*ww + (1-k)*(1-ww)) *
                        c[i][j][k].dot(weight_v);
            }
        }
    }
    return accum;
}
