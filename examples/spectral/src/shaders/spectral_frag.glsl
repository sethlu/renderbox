
#version 330

const float screenGamma = 2.2;
const int intervals = 81; // number of 5nm data points
const int stride = 4; // steps of 5nm, less accuracy from longer strides

// Generated from util/black-body-radiation.py at 3000K over [380nm, 780nm] at 5nm intervals
const float[intervals] spectralRadiance3000 = float[](49641355760.65561, 54782546391.79315, 60253444582.97214, 66057572842.27418, 72197354478.52588, 78674111961.18173, 85488073133.5703, 92638384751.56671, 100123132779.6051, 107939368848.0003, 116083142258.71309, 124549536919.83142, 133332712590.95462, 142425949831.21384, 151821698057.72632, 161511626143.76752, 171486675011.8717, 181737111706.47937, 192252584462.8219, 203022178322.65823, 214034470882.6049, 225277587796.49863, 236739257689.00497, 248406866173.04074, 260267508698.19556, 272308041990.8451, 284515133878.8464, 296875311324.33606, 309375006517.1438, 322000600908.5048, 334738467090.0611, 347575008446.6155, 360496696532.57886, 373490106141.77057, 386541948057.96356, 399639099489.66406, 412768632206.8394, 425917838409.9933, 439074254373.0692, 452225681911.23004, 465360207732.86206, 478466220742.0008, 491532427363.2123, 504547864965.5456, 517501913465.86884, 530384305194.5986, 543185133108.75165, 555894857438.4049, 568504310853.1356, 581004702234.9077, 593387619143.2562, 605645029057.5292, 617769279479.4689, 629753096977.6083, 641589585252.8229, 653272222302.076, 664794856754.7933, 676151703453.6609, 687337338348.7683, 698346692771.1375, 709175047148.6808, 719818024224.644, 730271581835.5486, 740532005302.645, 750595899487.8909, 760460180562.5386, 770122067533.4703, 779579073569.663, 788828997168.315, 797869913197.5828, 806700163850.1771, 815318349539.6901, 823723319768.9905, 831914163997.8312, 839890202534.5233, 847650977474.4731, 855196243706.3495, 862525960004.7532, 869640280226.4414, 876539544625.4778, 883224271301.0271);
const float[intervals] spectralRadiance5000 = float[](7736286666203.863, 7996139882519.928, 8250894872078.483, 8500155880409.544, 8743563908325.207, 8980795946130.896, 9211564030433.402, 9435614152503.883, 9652725044854.57, 9862706870387.98, 10065399836213.215, 10260672752026.51, 10448421550843.064, 10628567787862.28, 10801057131359.607, 10965857857732.389, 11122959361188.467, 11272370687056.043, 11414119096309.318, 11548248667644.898, 11674818942303.041, 11793903615801.746, 11905589279833.047, 12009974216753.492, 12107167248378.977, 12197286640158.576, 12280459061248.67, 12356818600528.666, 12426505838187.336, 12489666972158.52, 12546452998388.955, 12597018943675.943, 12641523149611.586, 12680126606009.268, 12712992332061.828, 12740284803386.479, 12762169423043.504, 12778812034572.076, 12790378475063.527, 12797034166286.422, 12798943741887.754, 12796270708717.281, 12789177140354.746, 12777823400962.42, 12762367897634.654, 12742966859471.428, 12719774141662.748, 12692941052934.24, 12662616204769.463, 12628945380892.602, 12592071425562.773, 12552134149299.953, 12509270250730.652, 12463613253309.24, 12415293455736.46, 12364437894962.67, 12311170320725.691, 12255611180635.06, 12197877614874.049, 12138083459647.463, 12076339258558.945, 12012752281153.951, 11947426547914.828, 11880462861043.092, 11811958840409.316, 11742008964094.898, 11670704612990.95, 11598134118959.227, 11524382816096.188, 11449533094676.723, 11373664457386.777, 11296853577484.926, 11219174358561.98, 11140697995595.168, 11061493037018.719, 10981625447556.684, 10901158671586.174, 10820153696820.102, 10738669118117.773, 10656761201249.916, 10574483946461.785);

// Wood_Beam GDS363 Nw Pine 2X4 over [380nm, 780nm] at 5nm intervals
// Retrieved from: https://crustal.usgs.gov/speclab/data/HTMLmetadata/Wood_Beam_GDS363_Nw_Pine_2X4_ASDFRa_AREF.html
const float[intervals] spectralAlbedoNewPine = float[](0.06852904, 0.068654686, 0.068815708, 0.069045581, 0.069356449, 0.069748357, 0.070218541, 0.07073456, 0.071246661, 0.071711607, 0.072158493, 0.072658733, 0.073278554, 0.074022263, 0.074857384, 0.075752348, 0.076691672, 0.077669315, 0.078679949, 0.079726346, 0.080815956, 0.081957474, 0.083170354, 0.084480077, 0.085909151, 0.087446287, 0.089060031, 0.090721399, 0.092426524, 0.094185978, 0.096010014, 0.097902581, 0.099864066, 0.10189524, 0.10400217, 0.10619385, 0.10847745, 0.11083882, 0.11325144, 0.11569083, 0.11815475, 0.12065388, 0.12319801, 0.12578651, 0.1284121, 0.13106821, 0.13375343, 0.13646957, 0.13921785, 0.14198981, 0.14477266, 0.1475531, 0.15032147, 0.15307035, 0.15579219, 0.15848304, 0.16114105, 0.16376342, 0.16634281, 0.16886805, 0.17132968, 0.1737269, 0.1760651, 0.17834862, 0.18057476, 0.18273555, 0.18482468, 0.18684274, 0.18879549, 0.19068824, 0.19252598, 0.19431359, 0.19605534, 0.19774944, 0.19939078, 0.20097513, 0.20250611, 0.20399264, 0.20544276, 0.2068581, 0.20823671);

uniform vec3 sceneAmbientColor;
uniform vec3 vertexColor;

out vec4 fragmentColor;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

void addSpectrum(out float c[intervals], float a[intervals], float b[intervals]) {
    for (int i = 0; i < intervals; ++i) {
        c[i] = a[i] + b[i];
    }
}

void mulSpectrum(inout float a[intervals], float b) {
    for (int i = 0; i < intervals; ++i) {
        a[i] = a[i] * b;
    }
}

void mulSpectrum(out float c[intervals], float a[intervals], float b[intervals]) {
    for (int i = 0; i < intervals; ++i) {
        c[i] = a[i] * b[i];
    }
}

void mulSpectrum(out float c[intervals], float a[intervals], float b) {
    for (int i = 0; i < intervals; ++i) {
        c[i] = a[i] * b;
    }
}

// Table from: http://www.fourmilab.ch/documents/specrend/
const float cieColorMatch[81 * 3] = float[81 * 3](
    0.0014, 0.0000, 0.0065,
    0.0022, 0.0001, 0.0105,
    0.0042, 0.0001, 0.0201,
    0.0076, 0.0002, 0.0362,
    0.0143, 0.0004, 0.0679,
    0.0232, 0.0006, 0.1102,
    0.0435, 0.0012, 0.2074,
    0.0776, 0.0022, 0.3713,
    0.1344, 0.0040, 0.6456,
    0.2148, 0.0073,1.0391,
    0.2839, 0.0116,1.3856,
    0.3285, 0.0168,1.6230,
    0.3483, 0.0230,1.7471,
    0.3481, 0.0298,1.7826,
    0.3362, 0.0380,1.7721,
    0.3187, 0.0480,1.7441,
    0.2908, 0.0600,1.6692,
    0.2511, 0.0739,1.5281,
    0.1954, 0.0910,1.2876,
    0.1421, 0.1126,1.0419,
    0.0956, 0.1390, 0.8130,
    0.0580, 0.1693, 0.6162,
    0.0320, 0.2080, 0.4652,
    0.0147, 0.2586, 0.3533,
    0.0049, 0.3230, 0.2720,
    0.0024, 0.4073, 0.2123,
    0.0093, 0.5030, 0.1582,
    0.0291, 0.6082, 0.1117,
    0.0633, 0.7100, 0.0782,
    0.1096, 0.7932, 0.0573,
    0.1655, 0.8620, 0.0422,
    0.2257, 0.9149, 0.0298,
    0.2904, 0.9540, 0.0203,
    0.3597, 0.9803, 0.0134,
    0.4334, 0.9950, 0.0087,
    0.5121, 1.0000, 0.0057,
    0.5945, 0.9950, 0.0039,
    0.6784, 0.9786, 0.0027,
    0.7621, 0.9520, 0.0021,
    0.8425, 0.9154, 0.0018,
    0.9163, 0.8700, 0.0017,
    0.9786, 0.8163, 0.0014,
    1.0263, 0.7570, 0.0011,
    1.0567, 0.6949, 0.0010,
    1.0622, 0.6310, 0.0008,
    1.0456, 0.5668, 0.0006,
    1.0026, 0.5030, 0.0003,
    0.9384, 0.4412, 0.0002,
    0.8544, 0.3810, 0.0002,
    0.7514, 0.3210, 0.0001,
    0.6424, 0.2650, 0.0000,
    0.5419, 0.2170, 0.0000,
    0.4479, 0.1750, 0.0000,
    0.3608, 0.1382, 0.0000,
    0.2835, 0.1070, 0.0000,
    0.2187, 0.0816, 0.0000,
    0.1649, 0.0610, 0.0000,
    0.1212, 0.0446, 0.0000,
    0.0874, 0.0320, 0.0000,
    0.0636, 0.0232, 0.0000,
    0.0468, 0.0170, 0.0000,
    0.0329, 0.0119, 0.0000,
    0.0227, 0.0082, 0.0000,
    0.0158, 0.0057, 0.0000,
    0.0114, 0.0041, 0.0000,
    0.0081, 0.0029, 0.0000,
    0.0058, 0.0021, 0.0000,
    0.0041, 0.0015, 0.0000,
    0.0029, 0.0010, 0.0000,
    0.0020, 0.0007, 0.0000,
    0.0014, 0.0005, 0.0000,
    0.0010, 0.0004, 0.0000,
    0.0007, 0.0002, 0.0000,
    0.0005, 0.0002, 0.0000,
    0.0003, 0.0001, 0.0000,
    0.0002, 0.0001, 0.0000,
    0.0002, 0.0001, 0.0000,
    0.0001, 0.0000, 0.0000,
    0.0001, 0.0000, 0.0000,
    0.0001, 0.0000, 0.0000,
    0.0000, 0.0000, 0.0000
);

// Table from: http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
const mat3 cieXyzToRgb = mat3(
    2.3706743, -0.5138850, 0.0052982,
    -0.9000405, 1.4253036, -0.0146949,
    -0.4706338, 0.0885814, 1.0093968
);

const float factor = 1e-10 * intervals / (intervals / stride);

void main() {

    vec3 lightWorldPosition = vec3(2, 2, 20);

    // Calculate spectrum

    vec3 N = normalize(vertexWorldNormal);
    vec3 L = normalize(lightWorldPosition - vertexWorldPosition);
    float distance = length(lightWorldPosition - vertexWorldPosition);

    float diffuseColor[intervals];
    mulSpectrum(diffuseColor, spectralRadiance3000, spectralAlbedoNewPine);
    mulSpectrum(diffuseColor, max(dot(N, L), 0) / (1 + (0.25 * distance * distance)));

    float spectrum[intervals] = diffuseColor;

    // Spectrum to rgb
    // Adapted from: http://www.fourmilab.ch/documents/specrend/

    float X = 0, Y = 0, Z = 0;
    for (int i = 0; i < intervals; i += stride) {
        float me = spectrum[i];
        X += me * cieColorMatch[i * 3];
        Y += me * cieColorMatch[i * 3 + 1];
        Z += me * cieColorMatch[i * 3 + 2];
    }

    vec3 xyz = vec3(X, Y, Z) * factor;
    vec3 rgb = cieXyzToRgb * xyz;

    // Gamma correction

    vec3 colorGammaCorrected = pow(rgb, vec3(1 / screenGamma));
    fragmentColor = vec4(colorGammaCorrected, 1);

}