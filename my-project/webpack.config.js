var path = require('path')
var webpack = require('webpack')

const contentBase = path.resolve(__dirname);

module.exports = {
  entry: './src/main.js',
  output: {
    path: path.resolve(__dirname, './dist'),
    publicPath: '/dist/',
    filename: 'build.js'
  },
  node: {
    fs: "empty"
  },
  /*mime: {
    'application/wasm': ['wasm']
  },*/
  module: {
    rules: [
      // Emscripten JS files define a global. With `exports-loader` we can 
      // load these files correctly (provided the global’s name is the same
      // as the file name).
      /*{
        test: /hello-func-cpp-wasm\.js$/,
        //type: 'js',
        //test: /hello-func-cpp-wasm\.js$/,
        loader: "exports-loader"
      },
      // wasm files should not be processed but just be emitted and we want
      // to have their public URL.
      {
        //test: /hello-func-cpp-wasm\.wasm$/,
        test: /hello-func-cpp-wasm\.wasm$/,
        //type: 'wasm',
        //loaders: ['wasm-loader']
        loader: "file-loader",
        options: {
          //type: "javascript/auto"
          type: "wasm",
          //type: "javascript/auto",
          publicPath: "dist/"
        }
      },*/
      {
        test: /\.css$/,
        use: [
          'vue-style-loader',
          'css-loader'
        ],
      },      {
        test: /\.vue$/,
        loader: 'vue-loader',
        options: {
          loaders: {
          }
          // other vue-loader options go here
        }
      },
      {
        test: /\.js$/,
        loader: 'babel-loader',
        exclude: /node_modules/
      },
      {
        test: /\.(png|jpg|gif|svg)$/,
        loader: 'file-loader',
        options: {
          name: '[name].[ext]?[hash]'
        }
      }
    ]
  },
  resolve: {
    alias: {
      'vue$': 'vue/dist/vue.esm.js'
    },
    extensions: ['*', '.js', '.vue', '.json']
  },
  devServer: {
    historyApiFallback: true,
    noInfo: true,
    overlay: true,
    /*contentBase: [contentBase],
    publicPath: '/dist',
    watchContentBase: true,
    // webpack-dev-server middleware
    before(app) {
        // =============================================
        // use proper mime-type for wasm files
        // =============================================
        app.get('*.wasm', (req, res, next) => {
            var options = {
                root: contentBase,
                dotfiles: 'deny',
                headers: {
                    'Content-Type': 'application/wasm'
                }
            };

            res.sendFile(req.url, options, (err) => {
                if (err) {
                    next(err);
                }
            });
        });

        console.log('');
        console.log(' [examples] wasm mime-type handler ready');
        console.log('');

  }*/
  },
  performance: {
    hints: false
  },
  devtool: '#eval-source-map'
}

if (process.env.NODE_ENV === 'production') {
  module.exports.devtool = '#source-map'
  // http://vue-loader.vuejs.org/en/workflow/production.html
  module.exports.plugins = (module.exports.plugins || []).concat([
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: '"production"'
      }
    }),
    new webpack.optimize.UglifyJsPlugin({
      sourceMap: true,
      compress: {
        warnings: false
      }
    }),
    new webpack.LoaderOptionsPlugin({
      minimize: true
    })
  ])
}
